package labsoft.homeelderaid;

/**
 * Created by Luiz on 18/11/2016.
 */

public enum Pins {
    LightSensor("V14"),
    GasSensor("V6"),
    Presence("V30"),
    DoorSensor("V7"),
    LightActuator("V3"),
    GasActuator("V8"),
    Alarm("V9"),
    ConLight("V19"),
    ConGas("V10"),
    ConDoor("V11");

    public final String value;

    Pins(String value) {
        this.value = value;
    }
}
