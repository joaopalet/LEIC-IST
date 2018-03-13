package mmt;

import mmt.exceptions.BadDateSpecificationException;
import mmt.exceptions.BadEntryException;
import mmt.exceptions.BadTimeSpecificationException;
import mmt.exceptions.InvalidPassengerNameException;
import mmt.exceptions.NoSuchDepartureException;
import mmt.exceptions.NoSuchPassengerIdException;
import mmt.exceptions.NoSuchServiceIdException;
import mmt.exceptions.NoSuchStationNameException;
import mmt.exceptions.NoSuchItineraryChoiceException;
import mmt.exceptions.NonUniquePassengerNameException;
import mmt.exceptions.ImportFileException;

import java.io.Serializable;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.File;
import java.io.FileNotFoundException;

import java.util.Collection;
import java.util.Collections;
import java.util.TreeMap;
import java.util.HashMap;
import java.util.Map;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

import java.time.LocalTime;
import java.time.LocalDate;


/**
 * A train company has schedules (services) for its trains and passengers that
 * acquire itineraries based on those schedules.
 */
public class TrainCompany implements Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201708301010L;


  /** Passenger counter. */
  private int _passengerNumber = 0;


  /** Passengers. */
  private Map<Integer, Passenger> _passengers = new TreeMap<Integer, Passenger>();


  /** Services. */
  private Map<Integer, Service> _services = new TreeMap<Integer, Service>();


  /** Stations. */
  private Map<String, Station> _stations = new HashMap<String, Station>();


  /** Auxiliar list of itineraries */
  private List<Itinerary> _auxItineraries = new ArrayList<Itinerary>();


  /*
   *
   * Constructors
   *
   */
  /*----------------------------------------------------------------------------------------------*/

  /**
   * Constructor
   */
  public TrainCompany() {}

  /**
   * Another constructor for this class
   * @param services
   * @param stations
   */
  public TrainCompany(Map<Integer, Service> services, Map<String, Station> stations) {
    _services = services;
    _stations = stations;
  }

  /*----------------------------------------------------------------------------------------------*/


  /**
   * getServices
   * @return all services.
   */
  public Map<Integer, Service> getServices() {
    return _services;
  }

  /*----------------------------------------------------------------------------------------------*/


  /**
   * getStations
   * @return all stations.
   */
  public Map<String, Station> getStations() {
    return _stations;
  }

  /*----------------------------------------------------------------------------------------------*/


  /**
   * importFile
   * @param filemane
   * @throws IOException
   * @throws ImportFileException If the file is not correct.
   */
  void importFile(String filename) throws IOException, ImportFileException {
    BufferedReader reader = new BufferedReader(new FileReader(filename));

    String line;

    while ((line = reader.readLine()) != null) {

      String[] fields = line.split("\\|");

      try {
        registerFromFields(fields);
      }
      catch (BadEntryException e)   {throw new ImportFileException(e);}
      catch (NoSuchPassengerIdException e)  {throw new ImportFileException(e);}
      catch (NoSuchServiceIdException e)   {throw new ImportFileException(e);}
      catch (NoSuchStationNameException e)  {throw new ImportFileException(e);}
      catch (NonUniquePassengerNameException e)  {throw new ImportFileException(e);}
    }
    reader.close();
  }

  /*----------------------------------------------------------------------------------------------*/


  /**
   * registerFromFields
   * @param fields Line of the file to be imported.
   * @throws NonUniquePassengerNameException
   * @throws BadEntryException
   * @throws NoSuchServiceIdException
   * @throws NoSuchStationNameException
   * @throws NoSuchPassengerIdException
   */
  void registerFromFields(String[] fields) throws NonUniquePassengerNameException, BadEntryException,
                                                  NoSuchServiceIdException, NoSuchStationNameException,
                                                  NoSuchPassengerIdException {

    // Regular expression pattern to match a passenger.
    Pattern patPassenger = Pattern.compile("^(PASSENGER)");
    // Regular expression pattern to match a service.
    Pattern patService = Pattern.compile("^(SERVICE)");
    // Regular expression pattern to match an itinerary.
    Pattern parItinerary = Pattern.compile("^(ITINERARY)");

    if (patPassenger.matcher(fields[0]).matches()) {
      registerPassenger(fields[1]);
    }
    else if (patService.matcher(fields[0]).matches()) {
      registerService(fields);
    }
    else if (parItinerary.matcher(fields[0]).matches()) {
      registerItinerary(fields);
    }
    else {
      //throw new BadEntryException(fields[0]);
    }
  }

  /*----------------------------------------------------------------------------------------------*/


  /*
   *
   * Operations with passengers.
   *
   */
  /*----------------------------------------------------------------------------------------------*/

  /**
   * registerPassenger
   * @param name
   * @throws NonUniquePassengerNameException
   */
   public void registerPassenger(String name) throws NonUniquePassengerNameException {
    for(Passenger p : _passengers.values())
      if(p.getName().equals(name))
        throw new NonUniquePassengerNameException(name);

    Passenger p = new Passenger(_passengerNumber, name);
    _passengers.put(_passengerNumber, p);
    _passengerNumber++;
  }

  /*----------------------------------------------------------------------------------------------*/

  /**
   * updatePassengerName
   * @param id
   * @param newName
   * @throws NoSuchPassengerIdException
   */
  public void updatePassengerName(int id, String newName) throws NoSuchPassengerIdException {
    Passenger p = _passengers.get(id);
    if(p == null)
      throw new NoSuchPassengerIdException(id);
    p.setName(newName);
  }

  /*----------------------------------------------------------------------------------------------*/

  /**
   * getAllPassengers
   * @return the desired Collection.
   */
  public Collection<Passenger> getAllPassengers() {
    return _passengers.values();
  }

  /*----------------------------------------------------------------------------------------------*/

  /**
   * getPassengerById
   * @param id
   * @throws NoSuchPassengerIdException
   * @return the desired passenger.
   */
  public Passenger getPassengerById(int id) throws NoSuchPassengerIdException {
    Passenger p = _passengers.get(id);
    if(p == null)
      throw new NoSuchPassengerIdException(id);
    return p;
  }

  /*----------------------------------------------------------------------------------------------*/

  /**
   * getItinerariesByPassenger
   * @return list with strings of all itineraries by passenger.
   */
  public List<String> getItinerariesByPassenger() {
    List<String> strings = new ArrayList<String>();

    for(Passenger p : _passengers.values())
      if(p.hasItineraries())
        strings.add(p.presentItineraries());

    return strings;
  }

  /*----------------------------------------------------------------------------------------------*/


  /*
   *
   * Operations with services.
   *
   */
  /*----------------------------------------------------------------------------------------------*/

  /**
   * registerService
   * @param fields
   */
  public void registerService(String[] fields) {
    int size = fields.length;
    int id = Integer.parseInt(fields[1]);
    double cost = Double.parseDouble(fields[2]);
    Service service = new Service(id, cost);
    LocalTime time;

    for(int i = 3; i < size; i+=2) {
      time = LocalTime.parse(fields[i]);

      Station station = _stations.get(fields[i+1]);
      if(station == null) {
        station = new Station(fields[i+1]);
        _stations.put(fields[i+1], station);
      }

      service.addStation(time, station);

      if(i < size - 1)
        station.addDeparture(service, LocalTime.parse(fields[i]));
    }

    _services.put(id, service);
  }

  /*----------------------------------------------------------------------------------------------*/

  /**
   * getAllServices
   * @return Collection of all services.
   */
  public Collection<Service> getAllServices() {
    return _services.values();
  }

  /*----------------------------------------------------------------------------------------------*/

  /**
   * getServiceById
   * @param id
   * @throws NoSuchServiceIdException
   * @return the desired service
   */
  public Service getServiceById(int id) throws NoSuchServiceIdException{
    Service s = _services.get(id);
    if(s == null)
      throw new NoSuchServiceIdException(id);
    return s;
  }

  /*----------------------------------------------------------------------------------------------*/

  /**
   * getServicesByDeparture
   * @param name
   * @throws NoSuchStationNameException
   * @return Collection of services sorted by departure.
   */
  public Collection<Service> getServicesByDeparture(String name) throws NoSuchStationNameException {
    if (_stations.get(name) == null)
      throw new NoSuchStationNameException(name);

    List<Service> list = new ArrayList<Service>();

    for(Service s : _services.values())
      if (s.getDepartureStation().getName().equals(name))
        list.add(s);

    Collections.sort(list, Service.DEPARTURE_COMPARATOR);

    return list;
  }

  /*----------------------------------------------------------------------------------------------*/

  /**
   * getServicesByArrival
   * @param name
   * @throws NoSuchStationNameException
   * @return Collection of services sorted by arrival.
   */
  public Collection<Service> getServicesByArrival(String name) throws NoSuchStationNameException {
    if (_stations.get(name) == null)
      throw new NoSuchStationNameException(name);

    List<Service> list = new ArrayList<Service>();

    for(Service s : _services.values())
      if (s.getArrivalStation().getName().equals(name))
        list.add(s);

    Collections.sort(list, Service.ARRIVAL_COMPARATOR);

    return list;
  }

  /*----------------------------------------------------------------------------------------------*/


  /*
   *
   * Operations with itineraries.
   *
   */
  /*----------------------------------------------------------------------------------------------*/

  /**
   * registerItinerary
   * @param fields
   * @throws NoSuchServiceIdException
   * @throws NoSuchStationNameException
   * @throws NoSuchPassengerIdException
   */
  public void registerItinerary(String[] fields) throws NoSuchServiceIdException, NoSuchStationNameException,
                                                        NoSuchPassengerIdException {
    Itinerary itinerary;
    List<Segment> segments = new ArrayList<Segment>();
    int size = fields.length;
    String[] s;
    int serviceId;
    String depStationName;
    String arrStationName;

    int passengerId = Integer.parseInt(fields[1]);
    LocalDate date = LocalDate.parse(fields[2]);

    if(_passengers.get(passengerId) == null)
      throw new NoSuchPassengerIdException(passengerId);

    for(int i = 3; i < size; i++) {
      s = fields[i].split("\\/");
      serviceId = Integer.parseInt(s[0]);
      depStationName = s[1];
      arrStationName = s[2];

      Service service = _services.get(serviceId);
      Station depStation = _stations.get(depStationName);
      Station arrStation = _stations.get(arrStationName);

      if(service == null)
        throw new NoSuchServiceIdException(serviceId);

      if(depStation == null)
        throw new NoSuchStationNameException(depStationName);

      if(arrStation == null)
        throw new NoSuchStationNameException(arrStationName);

      segments.add(new Segment(service, depStation, arrStation));
    }

    Passenger passenger = _passengers.get(passengerId);
    itinerary = new Itinerary(date, segments);
    passenger.buyItinerary(itinerary);
  }

  /*----------------------------------------------------------------------------------------------*/

  /**
   * getPassengerItineraries
   * @param id
   * @throws NoSuchPassengerIdException
   * @return all itineraries of the desired passenger.
   */
  public String getPassengerItineraries(int id) throws NoSuchPassengerIdException {
    Passenger p = getPassengerById(id);

    if(p.hasItineraries())
      return p.presentItineraries();
    else
      return null;
  }

  /*----------------------------------------------------------------------------------------------*/

  /**
   * findItineraries
   * @param passengerId
   * @param departureStationName
   * @param arrivalStationName
   * @param departureTime
   * @throws NoSuchPassengerIdException
   * @throws NoSuchStationNameException
   * @throws BadDateSpecificationException
   * @throws BadTimeSpecificationException
   * @return list of strings of all valid itineraries.
   */
  public List<String> findItineraries(int passengerId, String departureStationName,
                                          String arrivalStationName, String departureDate,
                                          String departureTime) throws NoSuchPassengerIdException,
                                          NoSuchStationNameException, BadDateSpecificationException,
                                          BadTimeSpecificationException {

    Passenger passenger = _passengers.get(passengerId);
    if(passenger == null)
      throw new NoSuchPassengerIdException(passengerId);

    LocalDate date = LocalDate.parse(departureDate);
    if(date == null)
      throw new BadDateSpecificationException(departureDate);

    LocalTime time = LocalTime.parse(departureTime);
    if(time == null)
      throw new BadTimeSpecificationException(departureTime);

    Station departureStation = _stations.get(departureStationName);
    if(departureStation == null)
      throw new NoSuchStationNameException(departureStationName);

    Station arrivalStation = _stations.get(arrivalStationName);
    if(arrivalStation == null)
      throw new NoSuchStationNameException(arrivalStationName);

    _auxItineraries = new ArrayList<Itinerary>();
    List<Itinerary> allItineraries =    new ArrayList<Itinerary>();
    List<String> validItineraries =  new ArrayList<String>();
    List<Service> services =            new ArrayList<Service>(_services.values());
    int i = 1;

    allItineraries = findAllItineraries(departureStation, departureStation, arrivalStation, date, time, departureStation,
                                        new ArrayList<Segment>(), null, new ArrayList<Itinerary>());

    Collections.sort(allItineraries, Itinerary.GLOBAL_COMPARATOR);

    for(Itinerary itinerary : allItineraries) {
      String string = new String();
      Service firstService =  itinerary.getFirstService();

      if(services.contains(firstService)) {
        services.remove(firstService);

        _auxItineraries.add(itinerary);

        string += "Itinerário " + (i++) + " para " + itinerary.getDate() + " @ "
                  + String.format("%.2f", itinerary.getCost()) + itinerary.toString();

        validItineraries.add(string);
      }
    }

    return validItineraries;
  }

  /*----------------------------------------------------------------------------------------------*/

  /**
   * findAllItineraries
   * @param departureStation
   * @param currentStation
   * @param destinyStation
   * @param departureDate
   * @param currentTime
   * @param inStation
   * @param segments
   * @param currentService
   * @param itineraries
   * @return list of all itineraries found.
   */
  public List<Itinerary> findAllItineraries(Station departureStation, Station currentStation,
                                          Station destinyStation, LocalDate departureDate,
                                          LocalTime currentTime, Station inStation, List<Segment> segments,
                                          Service currentService, List<Itinerary> itineraries) {

    for(Service nextService : currentStation.getCorrespondingServices()) {

      // We are at the starting point.
      if(currentStation == departureStation)
        currentService = nextService;

      LocalTime departureTime = currentStation.getDepartureTime(nextService);

      // We are on time
      if(departureTime.compareTo(currentTime) >= 0) {

        // We have a direct route (itinerary found).
        if(nextService.containsStationsInOrder(currentStation, destinyStation)) {

          // We have switched service.
          if(nextService != currentService)
            segments.add(new Segment(currentService, inStation, currentStation));

          segments.add(new Segment(nextService, currentStation, destinyStation));
          itineraries.add(new Itinerary(departureDate, segments));

          segments = new ArrayList<Segment>();
        }

        // We have no direct route.
        else {

          Station nextStation = nextService.getStationAfter(departureTime);

          // There are still departures from the station we're at.
          if(nextStation != null) {
            LocalTime arrivalTime = nextStation.getDepartureTime(nextService);

            // We will switch service.
            if(nextService != currentService) {
              segments.add(new Segment(currentService, inStation, currentStation));
              findAllItineraries(departureStation, nextStation, destinyStation, departureDate,
                                        arrivalTime, currentStation, segments, nextService, itineraries);
            }

            // We are staying on the same service.
            else
              findAllItineraries(departureStation, nextStation, destinyStation, departureDate,
                                        arrivalTime, inStation, segments, nextService, itineraries);
          }
        }
      }
    }

    return itineraries;
  }

  /*----------------------------------------------------------------------------------------------*/

  /**
   * commitItinerary
   * @param passengerId
   * @param itineraryNumber
   * @throws NoSuchItineraryChoiceException
   */
  public void commitItinerary(int passengerId, int itineraryNumber) throws NoSuchItineraryChoiceException{
    if(itineraryNumber > _auxItineraries.size() || itineraryNumber < 0)
      throw new NoSuchItineraryChoiceException(passengerId, itineraryNumber);

    Itinerary itinerary = _auxItineraries.get(itineraryNumber - 1);

    _passengers.get(passengerId).buyItinerary(itinerary);
  }

  /*----------------------------------------------------------------------------------------------*/

}
