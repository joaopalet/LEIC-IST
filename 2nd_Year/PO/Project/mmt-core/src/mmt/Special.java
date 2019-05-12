package mmt;

  public class Special extends Category {

  /**
   * Constructors
   */
  public Special(Passenger p) {
    super(p, 0.50);
  }


  /**
   * updateState
   */
  public void updateState() {
    double sum = getValueSpentLast10();

    if (sum <= 250.00)
      _passenger.setCategory(new Normal(_passenger));

    else if (sum <= 2500.00)
      _passenger.setCategory(new Frequent(_passenger));
  }


  /**
   * getName
   */
  public String getName() {
    return "ESPECIAL";
  }

}
