package mmt.app.passenger;

import mmt.TicketOffice;
import mmt.app.exceptions.BadPassengerNameException;//nao usar
import mmt.app.exceptions.DuplicatePassengerNameException;
import mmt.exceptions.InvalidPassengerNameException;//nao usar
import mmt.exceptions.NonUniquePassengerNameException;//nao usar
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

//FIXME import other classes if necessary

/**
 * §3.3.3. Register passenger.
 */
public class DoRegisterPassenger extends Command<TicketOffice> {

  /** Input field. */
  Input<String> _name;


  /**
   * @param receiver
   */
  public DoRegisterPassenger(TicketOffice receiver) {
    super(Label.REGISTER_PASSENGER, receiver);
    _name = _form.addStringInput(Message.requestPassengerName());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try {
      _receiver.addPassenger(_name.value());
    }
    catch (NonUniquePassengerNameException e) {
      throw new DuplicatePassengerNameException(_name.value());
    }
  }
}
