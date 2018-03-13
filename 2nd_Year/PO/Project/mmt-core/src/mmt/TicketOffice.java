package mmt;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import mmt.exceptions.BadDateSpecificationException;
import mmt.exceptions.BadTimeSpecificationException;
import mmt.exceptions.ImportFileException;
import mmt.exceptions.InvalidPassengerNameException;
import mmt.exceptions.MissingFileAssociationException;
import mmt.exceptions.NoSuchPassengerIdException;
import mmt.exceptions.NoSuchServiceIdException;
import mmt.exceptions.NoSuchStationNameException;
import mmt.exceptions.NoSuchItineraryChoiceException;
import mmt.exceptions.NonUniquePassengerNameException;

import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.TreeMap;
import java.util.Map;
import java.util.ArrayList;
import java.util.List;


/**
 * Façade for handling persistence and other functions.
 */
public class TicketOffice {

  /** The object doing most of the actual work. */
  private TrainCompany _trains = new TrainCompany();
  

  /** Associated file name. */
  private String _associatedFile = new String();


  /**
   * getAssociatedFile
   */
  /*----------------------------------------------------------------------------------------------*/

  public boolean hasAssociatedFile() {
    return !_associatedFile.isEmpty();
  }

  /*----------------------------------------------------------------------------------------------*/


  /*
   *
   * Reset, Save, Load and Import.
   *
   */
  /*----------------------------------------------------------------------------------------------*/

  public void reset() {
    _trains = new TrainCompany(_trains.getServices(), _trains.getStations());
    _associatedFile = new String();
  }

  /*----------------------------------------------------------------------------------------------*/

  public void save(String filename) throws IOException {
    ObjectOutputStream oos =  new ObjectOutputStream(
                              new BufferedOutputStream(
                              new FileOutputStream(filename)));
    oos.writeObject(_trains);
    oos.close();

    _associatedFile = filename;
  }

  public void save() throws IOException {
    ObjectOutputStream oos =  new ObjectOutputStream(
                              new BufferedOutputStream(
                              new FileOutputStream(_associatedFile)));
    oos.writeObject(_trains);
    oos.close();
  }

  /*----------------------------------------------------------------------------------------------*/

  public void load(String filename) throws FileNotFoundException, IOException,
                                    ClassNotFoundException {
    ObjectInputStream ois = new ObjectInputStream(
                            new BufferedInputStream(
                            new FileInputStream(filename)));

    _trains = new TrainCompany();
    _trains = (TrainCompany) ois.readObject();
    ois.close();

    _associatedFile = filename;
  }

  /*----------------------------------------------------------------------------------------------*/

  public void importFile(String datafile) throws ImportFileException {
    try   {_trains.importFile(datafile);}
    catch(IOException e)   {throw new ImportFileException(e);}
    catch(ImportFileException e)  {throw new ImportFileException(e);}
  }

  /*----------------------------------------------------------------------------------------------*/


  /*
   *
   * Operations with passengers.
   *
   */
  /*----------------------------------------------------------------------------------------------*/

  public void addPassenger(String name) throws NonUniquePassengerNameException {
    _trains.registerPassenger(name);
  }

  /*----------------------------------------------------------------------------------------------*/

  public void changePassengerName(int id, String newName) throws NoSuchPassengerIdException {
    _trains.updatePassengerName(id, newName);
  }

  /*----------------------------------------------------------------------------------------------*/

  public ArrayList<String> showAllPassengers() {
    ArrayList<String> allPassengers = new ArrayList<String>();

    for(Passenger p : _trains.getAllPassengers())
      allPassengers.add(p.toString());

    return allPassengers;
  }

  /*----------------------------------------------------------------------------------------------*/

  public String showPassengerById(int id) throws NoSuchPassengerIdException{
    return _trains.getPassengerById(id).toString();
  }

  /*----------------------------------------------------------------------------------------------*/


  /*
   *
   * Operations with services.
   *
   */
  /*----------------------------------------------------------------------------------------------*/

  public ArrayList<String> showAllServices() {
    ArrayList<String> allServices = new ArrayList<String>();

    for(Service s : _trains.getAllServices())
      allServices.add(s.toString());

    return allServices;
  }

  /*----------------------------------------------------------------------------------------------*/

  public String showServiceById(int id) throws NoSuchServiceIdException {
    return _trains.getServiceById(id).toString();
  }

  /*----------------------------------------------------------------------------------------------*/

  public ArrayList<String> showServicesByDeparture(String name) throws NoSuchStationNameException {
    ArrayList<String> servicesDepartingFrom = new ArrayList<String>();

    for(Service s : _trains.getServicesByDeparture(name))
      servicesDepartingFrom.add(s.toString());

    return servicesDepartingFrom;
  }

  /*----------------------------------------------------------------------------------------------*/

  public ArrayList<String> showServicesByArrival(String name) throws NoSuchStationNameException {
    ArrayList<String> servicesArrivingAt = new ArrayList<String>();

    for(Service s : _trains.getServicesByArrival(name))
      servicesArrivingAt.add(s.toString());

    return servicesArrivingAt;
  }

  /*----------------------------------------------------------------------------------------------*/


  /*
   *
   * Operations with itineraries.
   *
   */
  /*----------------------------------------------------------------------------------------------*/

  public List<String> showAllItineraries() {
    return _trains.getItinerariesByPassenger();
  }

  /*----------------------------------------------------------------------------------------------*/

  public String showPassengerItineraries(int id) throws NoSuchPassengerIdException {
    return _trains.getPassengerItineraries(id);
  }

  /*----------------------------------------------------------------------------------------------*/

  public List<String> searchItineraries(int passengerId, String departureStation, String arrivalStation,
                                String departureDate, String departureTime) throws
                                NoSuchPassengerIdException, NoSuchStationNameException,
                                BadDateSpecificationException, BadTimeSpecificationException {

    return _trains.findItineraries(passengerId, departureStation, arrivalStation,
                                                    departureDate, departureTime);
  }

  /*----------------------------------------------------------------------------------------------*/

  public void commitItinerary(int passengerId, int itineraryNumber) throws NoSuchItineraryChoiceException {
    _trains.commitItinerary(passengerId, itineraryNumber);
  }

  /*----------------------------------------------------------------------------------------------*/

}
