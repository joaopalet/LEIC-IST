package mmt;

import java.io.Serializable;
import java.util.Collection;
import java.util.Collections;
import java.util.ArrayList;
import java.util.List;
import java.util.Comparator;

import mmt.exceptions.NonUniquePassengerNameException;

import java.time.LocalTime;
import java.time.LocalDate;


public class Passenger implements Serializable {
  /** Passenger ID */
  private long _id;

  /** Passenger name */
  private String _name;

  /** Passenger category */
  private Category _category;

  /** Total value spent */
  private double _valueSpent = 0;

  /** Hours spent travelling */
  private int _accumulatedHours = 0;

  /** Minutes spent travelling (hours out) */
  private int _accumulatedMinutes = 0;

  /** Itinerary counter */
  private int _itineraryNumber = 1;

  /** Passenger itineraries */
  private List<Itinerary> _itineraries = new ArrayList<Itinerary>();


  /**
   * Constructor
   */
  public Passenger(long id, String name) {
    _id = id;
    _name = name;
    _category = new Normal(this);
  }


  /*
   *
   * Getters
   *
   */
  public String getName() {
    return _name;
  }

  public long getId() {
    return _id;
  }

  public int getItineraryNumber() {
    return _itineraryNumber;
  }

  public Itinerary getItinerary(int i) {
    return _itineraries.get(i);
  }


  /*
   *
   * Setters
   *
   */
  protected void setCategory(Category category) {
    _category = category;
  }

  public void setName(String name) {
    _name = name;
  }

  public void addTravelTime(long hours, long minutes) {
    _accumulatedMinutes += minutes;
    if(_accumulatedMinutes > 59) {
      _accumulatedMinutes = _accumulatedMinutes % 60;
      _accumulatedHours++;
    }

    _accumulatedHours += hours;
  }


  /**
   * buyItinerary
   */
  public void buyItinerary(Itinerary itinerary) {
    double price = itinerary.getCost() * _category.getDiscount();
    int timeInMinutes = itinerary.getDurationMinutes();
    long hours = timeInMinutes / 60;
    long minutes = timeInMinutes % 60;

    _itineraries.add(itinerary);

    _valueSpent += price;
    addTravelTime(hours, minutes);

    _category.updateState();

    _itineraryNumber++;
  }


  /**
   * hasItineraries
   */
  public boolean hasItineraries() {
    return !_itineraries.isEmpty();
  }


  /**
   * presentItineraries
   */
  public String presentItineraries() {
    int i = 1;

    List<Itinerary> list = new ArrayList<>(_itineraries);

    Collections.sort(list, Itinerary.DATE_COMPARATOR);

    String string = "== Passageiro " + getId() + " : " + getName() + " ==";

    for(Itinerary itinerary : list)
      string += "\n\nItinerário " + (i++) + " para " + itinerary.getDate() + " @ "
                + String.format("%.2f", itinerary.getCost()) + itinerary.toString();

    return string;
  }
  

  /**
   * toString
   */
  @SuppressWarnings("nls")
  @Override
  public String toString() {
    return getId() + "|" + getName() + "|" + _category.getName() + "|" + _itineraries.size()
          + "|" +  String.format("%.2f", _valueSpent) + "|" + String.format("%02d", _accumulatedHours)
          + ":" + String.format("%02d", _accumulatedMinutes);
  }

}
