package mmt.app.main;

import java.io.IOException;

import mmt.TicketOffice;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;

//FIXME import other classes if necessary

/**
 * §3.1.1. Save to file under current name (if unnamed, query for name).
 */
public class DoSave extends Command<TicketOffice> {

  /** Input fields. */
  Input<String> _requestedFile;

  /**
   * @param receiver
   */
  public DoSave(TicketOffice receiver) {
    super(Label.SAVE, receiver);

      _requestedFile = _form.addStringInput(Message.newSaveAs());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {

    try {
      if(_receiver.hasAssociatedFile())
        _receiver.save();
      else {
      _form.parse();
      _receiver.save(_requestedFile.value());
    }
    }
    catch (IOException e) {
      System.out.println("Erro : " + _requestedFile.value() + " : " + e);
    }
  }
}
