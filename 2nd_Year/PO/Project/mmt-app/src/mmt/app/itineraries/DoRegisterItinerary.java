package mmt.app.itineraries;

import mmt.TicketOffice;
import mmt.app.exceptions.BadDateException;
import mmt.app.exceptions.BadTimeException;
import mmt.app.exceptions.NoSuchItineraryException;
import mmt.app.exceptions.NoSuchPassengerException;
import mmt.app.exceptions.NoSuchStationException;
import mmt.exceptions.BadDateSpecificationException;
import mmt.exceptions.BadTimeSpecificationException;
import mmt.exceptions.NoSuchPassengerIdException;
import mmt.exceptions.NoSuchStationNameException;
import mmt.exceptions.NoSuchItineraryChoiceException;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

import java.util.List;

//FIXME import other classes if necessary

/**
 * §3.4.3. Add new itinerary.
 */
public class DoRegisterItinerary extends Command<TicketOffice> {

  /** Input fields. */
  Input<Integer> _id;
  Input<String> _depStation;
  Input<String> _arrStation;
  Input<String> _depDate;
  Input<String> _depTime;
  Input<Integer> _itineraryChoice;

  /**
   * @param receiver
   */
  public DoRegisterItinerary(TicketOffice receiver) {
    super(Label.REGISTER_ITINERARY, receiver);

    _id = _form.addIntegerInput(Message.requestPassengerId());
    _depStation = _form.addStringInput(Message.requestDepartureStationName());
    _arrStation = _form.addStringInput(Message.requestArrivalStationName());
    _depDate = _form.addStringInput(Message.requestDepartureDate());
    _depTime = _form.addStringInput(Message.requestDepartureTime());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    try {
      _form.parse();
      List<String> list = _receiver.searchItineraries(_id.value(), _depStation.value(),
                                                _arrStation.value(), _depDate.value(),
                                                _depTime.value());

      for(String s : list)
        _display.addLine("\n" + s);

      _display.display();

      if(!list.isEmpty()) {
        _form.clear();
        _itineraryChoice = _form.addIntegerInput(Message.requestItineraryChoice());
        _form.parse();

        if(_itineraryChoice.value() != 0) {
          _receiver.commitItinerary(_id.value(), _itineraryChoice.value());

          _form.clear();

          _id = _form.addIntegerInput(Message.requestPassengerId());
          _depStation = _form.addStringInput(Message.requestDepartureStationName());
          _arrStation = _form.addStringInput(Message.requestArrivalStationName());
          _depDate = _form.addStringInput(Message.requestDepartureDate());
          _depTime = _form.addStringInput(Message.requestDepartureTime());
        }
      }

    }
    catch (NoSuchPassengerIdException e) {
      _form.clear();

      _id = _form.addIntegerInput(Message.requestPassengerId());
      _depStation = _form.addStringInput(Message.requestDepartureStationName());
      _arrStation = _form.addStringInput(Message.requestArrivalStationName());
      _depDate = _form.addStringInput(Message.requestDepartureDate());
      _depTime = _form.addStringInput(Message.requestDepartureTime());

      throw new NoSuchPassengerException(e.getId());}
    catch (NoSuchStationNameException e) {
      _form.clear();

      _id = _form.addIntegerInput(Message.requestPassengerId());
      _depStation = _form.addStringInput(Message.requestDepartureStationName());
      _arrStation = _form.addStringInput(Message.requestArrivalStationName());
      _depDate = _form.addStringInput(Message.requestDepartureDate());
      _depTime = _form.addStringInput(Message.requestDepartureTime());

      throw new NoSuchStationException(e.getName());}
    catch (NoSuchItineraryChoiceException e) {
      _form.clear();

      _id = _form.addIntegerInput(Message.requestPassengerId());
      _depStation = _form.addStringInput(Message.requestDepartureStationName());
      _arrStation = _form.addStringInput(Message.requestArrivalStationName());
      _depDate = _form.addStringInput(Message.requestDepartureDate());
      _depTime = _form.addStringInput(Message.requestDepartureTime());

      throw new NoSuchItineraryException(e.getPassengerId(), e.getItineraryId());}
    catch (BadDateSpecificationException e) {
      _form.clear();

      _id = _form.addIntegerInput(Message.requestPassengerId());
      _depStation = _form.addStringInput(Message.requestDepartureStationName());
      _arrStation = _form.addStringInput(Message.requestArrivalStationName());
      _depDate = _form.addStringInput(Message.requestDepartureDate());
      _depTime = _form.addStringInput(Message.requestDepartureTime());

      throw new BadDateException(e.getDate());}
    catch (BadTimeSpecificationException e) {
      _form.clear();

      _id = _form.addIntegerInput(Message.requestPassengerId());
      _depStation = _form.addStringInput(Message.requestDepartureStationName());
      _arrStation = _form.addStringInput(Message.requestArrivalStationName());
      _depDate = _form.addStringInput(Message.requestDepartureDate());
      _depTime = _form.addStringInput(Message.requestDepartureTime());

      throw new BadTimeException(e.getTime());
    }
  }
}
