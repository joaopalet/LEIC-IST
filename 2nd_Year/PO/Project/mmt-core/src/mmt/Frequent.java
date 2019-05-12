package mmt;

public class Frequent extends Category {

  /**
   * Constructors
   */
  public Frequent(Passenger p) {
    super(p, 0.85);
  }
  

  /**
   * updateState
   */
  public void updateState() {
    double sum = getValueSpentLast10();

    if (sum <= 250.00)
      _passenger.setCategory(new Normal(_passenger));

    else if (sum > 2500.00)
      _passenger.setCategory(new Special(_passenger));
  }


  /**
   * getName
   */
  public String getName() {
    return "FREQUENTE";
  }

}
