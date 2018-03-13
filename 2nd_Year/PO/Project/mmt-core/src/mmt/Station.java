package mmt;

import java.io.Serializable;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.Comparator;

import java.time.LocalTime;
import java.time.LocalDate;


public class Station implements Serializable{

  /** Station name */
  private String _name;

  /** Departures board */
  private Map<Service, LocalTime> _departures = new HashMap<Service, LocalTime>();


  /**
   * Constructor
   */
  public Station(String name) {
    _name = name;
  }


  /*
   *
   * Getters
   *
   */
  public String getName() {
    return _name;
  }

  public LocalTime getDepartureTime(Service service) {
    return _departures.get(service);
  }

  public Set<Service> getCorrespondingServices() {
    return _departures.keySet();
  }


  /**
   * hasDepartures
   */
  /*public boolean hasDepartures() {
    return !_departures.isEmpty();
  }*/


  /**
   * addDeparture
   */
  public void addDeparture(Service service, LocalTime time) {
    _departures.put(service, time);
  }


  /**
   * toString
   */
  @SuppressWarnings("nls")
  @Override
  public String toString() {
    return getName();
  }

}
