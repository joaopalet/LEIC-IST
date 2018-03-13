package mmt;

import java.io.Serializable;
import java.util.Collection;
import java.util.Collections;
import java.util.TreeMap;
import java.util.Map;
import java.util.Comparator;
import java.util.Set;

import java.time.LocalTime;
import java.time.LocalDate;


public class Service implements Serializable {

  /** Service ID */
  private int _id;

  /** Service route */
  private TreeMap<LocalTime, Station> _route = new TreeMap<LocalTime, Station>();

  /** Total price */
  private double _price;

  /** Departure comparator */
  public final static Comparator<Service> DEPARTURE_COMPARATOR = new DepartureComparator();

  /** Arrival comparator */
  public final static Comparator<Service> ARRIVAL_COMPARATOR = new ArrivalComparator();


  /**
   * Constructor
   */
  public Service(int id, double price) {
    _id = id;
    _price = price;
  }


  /**
   * DepartureComparator
   */
  private static class DepartureComparator implements Comparator<Service> {
    @Override
    public int compare(Service s1, Service s2) {
      if (s1._route.firstKey().isAfter(s2._route.firstKey()))
        return 1;
      else if (s2._route.firstKey().isAfter(s1._route.firstKey()))
        return -1;
      else
        return 0;
    }
  }


  /**
   * ArrivalComparator
   */
  private static class ArrivalComparator implements Comparator<Service> {
    @Override
    public int compare(Service s1, Service s2) {
      if (s1._route.lastKey().isAfter(s2._route.lastKey()))
        return 1;
      else if (s2._route.lastKey().isAfter(s1._route.lastKey()))
        return -1;
      else
        return 0;
    }
  }


  /**
   *
   * Getters
   *
   */
  public Station getDepartureStation() {
    LocalTime key = _route.firstKey();
    return _route.get(key);
  }

  public Station getArrivalStation() {
    LocalTime key = _route.lastKey();
    return _route.get(key);
  }

  public Station getStationAt(LocalTime time) {
    return _route.get(time);
  }

  public Station getStationAfter(LocalTime time) {
    LocalTime key = _route.higherKey(time);
    if(key != null)
      return _route.get(key);
    else
      return null;
  }

  public LocalTime getArrivalTime() {
    return _route.lastKey();
  }

  public int getId() {
    return _id;
  }

  public double getPrice() {
    return _price;
  }

  public int getTotalDurationMinutes() {
    int t1 = _route.firstKey().getHour()*60 + _route.firstKey().getMinute();
    int t2 = _route.lastKey().getHour()*60 + _route.lastKey().getMinute();

    return t2 - t1;
  }

  public int getDurationBetweenStationsMinutes(Station s1, Station s2) {
    int t1 = s1.getDepartureTime(this).getHour()*60 + s1.getDepartureTime(this).getMinute();
    int t2 = s2.getDepartureTime(this).getHour()*60 + s2.getDepartureTime(this).getMinute();

    return t2 - t1;
  }

  public Map<LocalTime, Station> getSubMap(LocalTime t1, LocalTime t2) {
    return _route.subMap(t1, true, t2, true);
  }


  /**
   * addStation
   */
  public void addStation(LocalTime time, Station station) {
    _route.put(time, station);
  }


  /**
   * containsStationsInOrder
   */
  public boolean containsStationsInOrder(Station s1, Station s2) {
    if(!_route.containsValue(s1) || !_route.containsValue(s2))
      return false;

    boolean flag = false;

    for(Station station : _route.values()) {
      if(station == s1) {
        flag = true;
        break;
      }
      else if(station == s2) {
        flag = false;
        break;
      }
    }
    return flag;
  }


  /**
   * toString
   */
  @SuppressWarnings("nls")
  @Override
  public String toString() {
    String string = "Serviço #" + getId() + " @ " + String.format("%.2f", getPrice());

    Set<LocalTime> set = _route.keySet();
    for(LocalTime key : set){
      Station station = _route.get(key);
      string += "\n" + key + " " + station.getName();
    }

    return string;
  }

}
