package labsoft.homeelderaid;

/**
 * Created by Luiz on 18/11/2016.
 */

public enum Pins {
    Light("V0"),
    Gas("V1"),
    Presence("V2"),
    Door("V3"),
    LedGas(""),
    LedAlarm("");

    public final String value;

    Pins(String value) {
        this.value = value;
    }
}
