package mmt;

import java.io.Serializable;


public abstract class Category implements Serializable{

  /** Associated passenger */
  protected Passenger _passenger;

  /** The percentage of the price to pay */
  private double _discount;


  /**
   *Constructor
   */
  public Category(Passenger p, double d) {
    _passenger = p;
    _discount = d;
  }


  /*
   *
   * Getters
   *
   */
  public Passenger getPassenger() {
    return _passenger;
  }

  public double getDiscount() {
    return _discount;
  }


  /**
   * getValueSpentLast10
   */
  public double getValueSpentLast10() {
    double sum = 0;
    int itineraryNumber = _passenger.getItineraryNumber();

    for (int i = itineraryNumber -1; i > itineraryNumber - 11; i--) {
      if (i >= 0)
        sum += _passenger.getItinerary(i).getCost();
      else
        break;
    }

    return sum;
  }
  

  public abstract void updateState();

  public abstract String getName();

}
