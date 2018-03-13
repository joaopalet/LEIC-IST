package mmt.app.passenger;

import mmt.TicketOffice;
import mmt.app.exceptions.BadPassengerNameException;
import mmt.app.exceptions.DuplicatePassengerNameException;
import mmt.app.exceptions.NoSuchPassengerException;
import mmt.exceptions.InvalidPassengerNameException;
import mmt.exceptions.NoSuchPassengerIdException;
import mmt.exceptions.NonUniquePassengerNameException;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

//FIXME import other classes if necessary

/**
 * §3.3.4. Change passenger name.
 */
public class DoChangerPassengerName extends Command<TicketOffice> {

  /** Input fields. */
  Input<Integer> _id;
  Input<String> _name;


  /**
   * @param receiver
   */
  public DoChangerPassengerName(TicketOffice receiver) {
    super(Label.CHANGE_PASSENGER_NAME, receiver);

    _id = _form.addIntegerInput(Message.requestPassengerId());
    _name = _form.addStringInput(Message.requestPassengerName());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try {
      _receiver.changePassengerName(_id.value(), _name.value());
    }
    catch (NoSuchPassengerIdException e) {
      throw new NoSuchPassengerException(_id.value());
    }
    //FIXME falta a outra excepcao
  }
}
