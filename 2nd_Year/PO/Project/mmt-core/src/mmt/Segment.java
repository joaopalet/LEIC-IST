package mmt;

import java.io.Serializable;
import java.util.Collection;
import java.util.Collections;
import java.util.TreeMap;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Set;

import java.time.LocalTime;
import java.time.LocalDate;

public class Segment implements Serializable{

  /** Associated service */
  private Service _associatedService;

  /** Stops */
  private TreeMap<LocalTime, Station> _stops = new TreeMap<LocalTime, Station>();


  /**
   * Constructor
   */
  public Segment(Service service, Station origin, Station end) {
    _associatedService = service;
    LocalTime depTime = origin.getDepartureTime(service);
    LocalTime arrTime = end.getDepartureTime(service);

    Map<LocalTime, Station> map = service.getSubMap(depTime, arrTime);
    _stops.putAll(map);
  }


  /*
   *
   * Getters
   *
   */
  public Station getOriginStation() {
    return _stops.get(_stops.firstKey());
  }

  public Station getEndStation() {
    return _stops.get(_stops.lastKey());
  }

  public LocalTime getDepartureTime() {
    return _stops.firstKey();
  }

  public LocalTime getArrivalTime() {
    return _stops.lastKey();
  }

  public int getDurationMinutes() {
    return _associatedService.getDurationBetweenStationsMinutes(getOriginStation(), getEndStation());
  }

  public double getCost() {
    double cost = (getDurationMinutes()/(double) _associatedService.getTotalDurationMinutes()) * _associatedService.getPrice();

    return cost;
  }

  public Service getAssociatedService() {
    return _associatedService;
  }


  /**
   * toString
   */
  @SuppressWarnings("nls")
  @Override
  public String toString() {
    String string = "Serviço #" + _associatedService.getId() + " @ " + String.format("%.2f", getCost());

    Set<LocalTime> set = _stops.keySet();
    for(LocalTime key : set){
      Station station = _stops.get(key);
      string += "\n" + key + " " + station.getName();
    }

    return string;
  }

}
