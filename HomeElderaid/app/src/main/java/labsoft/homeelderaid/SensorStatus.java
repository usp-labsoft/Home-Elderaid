package labsoft.homeelderaid;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.widget.ImageView;
import android.widget.TextView;

/**
 * Created by Luiz on 01/12/2016.
 */

public class SensorStatus {
    public static final int LIGHT_SENSOR = 0;
    public static final int GAS_SENSOR = 0;
    public static final int DOOR_SENSOR = 0;

    public static MainActivity context;

    private static boolean isCon;

    public static void updSensorStatus (Pins pin, boolean status, MainActivity main){
        context = main;
        isCon = status;
        if (pin.equals(Pins.ConLight)) {
            updLightConHandler.sendEmptyMessage(0);
        } else if (pin.equals(Pins.ConGas)) {
            updGasConHandler.sendEmptyMessage(0);
        } else if (pin.equals(Pins.ConDoor)) {
            updDoorConHandler.sendEmptyMessage(0);
        }
    }

    static Handler updLightConHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            ImageView lightCon = (ImageView) context.findViewById(R.id.lightCon);
            if (isCon) {
                lightCon.setImageResource(android.R.drawable.presence_online);
            }
            else {
                lightCon.setImageResource(android.R.drawable.presence_offline);
            }
        }
    };

    static Handler updGasConHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            ImageView gasCon = (ImageView) context.findViewById(R.id.gasCon);
            if (isCon) {
                gasCon.setImageResource(android.R.drawable.presence_online);
            }
            else {
                gasCon.setImageResource(android.R.drawable.presence_offline);
            }
        }
    };

    static Handler updDoorConHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            ImageView doorCon = (ImageView) context.findViewById(R.id.doorCon);
            if (isCon) {
                doorCon.setImageResource(android.R.drawable.presence_online);
            }
            else {
                doorCon.setImageResource(android.R.drawable.presence_offline);
            }
        }
    };
}
