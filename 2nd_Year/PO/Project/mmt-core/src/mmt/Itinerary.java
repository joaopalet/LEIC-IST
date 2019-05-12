package mmt;

import java.io.Serializable;
import java.util.Collection;
import java.util.Collections;
import java.util.TreeMap;
import java.util.Map;
import java.util.Comparator;
import java.util.Set;
import java.util.List;
import java.util.ArrayList;

import java.time.LocalTime;
import java.time.LocalDate;

public class Itinerary implements Serializable{

  /** Date */
  private LocalDate _date;

  /** Segments of services */
  private List<Segment> _segments;

  /** Date comparator */
  public final static Comparator<Itinerary> DATE_COMPARATOR = new DateComparator();

  /** Global comparator */
  public final static Comparator<Itinerary> GLOBAL_COMPARATOR = new GlobalComparator();


  /**
   * Constructor
   */
  public Itinerary(LocalDate date, List<Segment> segments) {
    _date = date;
    _segments = segments;
  }


  /**
   * DateComparator
   */
  private static class DateComparator implements Comparator<Itinerary> {
    @Override
    public int compare(Itinerary i1, Itinerary i2) {
      if (i1.getDate().isAfter(i2.getDate()))
        return 1;
      else if (i2.getDate().isAfter(i1.getDate()))
        return -1;
      else
        return 0;
    }
  }


  /**
   * GlobalComparator
   */
  private static class GlobalComparator implements Comparator<Itinerary> {
    @Override
    public int compare(Itinerary i1, Itinerary i2) {
      if (i1.getDepartureTime().isAfter(i2.getDepartureTime()))
        return 1;
      else if (i2.getDepartureTime().isAfter(i1.getDepartureTime()))
        return -1;
      else {

        if (i1.getArrivalTime().isAfter(i2.getArrivalTime()))
          return 1;
        else if (i2.getArrivalTime().isAfter(i1.getArrivalTime()))
          return -1;
        else {

          if (i1.getCost()>i2.getCost())
            return 1;
          else if (i2.getCost()>i1.getCost())
            return -1;
          else
            return 0;
        }
      }
    }
  }


  /*
   *
   * Getters
   *
   */
   public double getCost() {
    double cost = 0;

    for(Segment segment : _segments) {
     cost += segment.getCost();
    }

    return cost;
  }

  public int getDurationMinutes() {
    LocalTime t1 = _segments.get(0).getDepartureTime();
    LocalTime t2 = _segments.get(_segments.size() - 1).getArrivalTime();

    int m1 = t1.getHour()*60 + t1.getMinute();
    int m2 = t2.getHour()*60 + t2.getMinute();

    return m2 -m1;
  }

  public LocalDate getDate() {
    return _date;
  }

  public LocalTime getDepartureTime() {
    return _segments.get(0).getDepartureTime();
  }

  public LocalTime getArrivalTime() {
    return _segments.get(_segments.size() - 1).getArrivalTime();
  }

  public Service getFirstService() {
    return _segments.get(0).getAssociatedService();
  }


  /**
   * toString
   */
  @SuppressWarnings("nls")
  @Override
  public String toString() {
    String string = new String();

    for(Segment segment : _segments)
      string += "\n" + segment;

    return string;
  }

}
