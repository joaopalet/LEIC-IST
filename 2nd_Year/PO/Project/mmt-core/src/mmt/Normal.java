package mmt;

public class Normal extends Category {

  /**
   * Constructor
   */
  public Normal(Passenger p) {
    super(p, 1.00);
  }

  /**
   * updateState
   */
  public void updateState() {
    double sum = getValueSpentLast10();

    if (sum > 2500.00)
      _passenger.setCategory(new Special(_passenger));

    else if (sum > 250.00)
      _passenger.setCategory(new Frequent(_passenger));
  }


  /**
   * getName
   */
  public String getName() {
    return "NORMAL";
  }

}
