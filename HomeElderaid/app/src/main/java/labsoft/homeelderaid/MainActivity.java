package labsoft.homeelderaid;

import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;

import android.os.AsyncTask;

import android.os.Bundle;
import android.util.TypedValue;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;

/**
 * Created by Luiz on 04/11/2016.
 */

public class MainActivity extends AppCompatActivity {
    // private String serverUrl = "http://blynk-cloud.com/";
    private static final String SERVERURL = "http://elderaid.me:8080/";

    private static final Long SLEEPTIME = 30000L;

    //private static final String authKey = "607cc39ee6de48708f1b831dacb40e37"; //Victor
    //private static final String authKey = "7fb8c29988504f5ab263b797a869827b"; //Alexis
    private SharedPreferences sp;
    private static String authKey;

    private static String json;
    private static String error;
    private static List<Pins> namesPins;
    private static List<Pins> conPins;

    private static List<Button> Buttons;
    //private Button lightButton, gasButton, doorButton;

    private static boolean isCon = false;
    private static double valueOfPinFromJson = 0.0;
    private static int opposeLightValue, alarmValue;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_principal);
        sp = getSharedPreferences("Login", 0);

        readAuthKeyFromPref();

        populateListNamesPins();

        populateButtonsWithListeners();

        new Thread( new Runnable(){
            @Override
            public void run(){
                while(true) {
                    updConStatus();

                    for (Pins pin : namesPins) {
                        readJsonFromURL(pin.value);
                        readValueOfPinFromJson();
                        updatePinView(pin);
                        json = null;
                    }

                    try {
                        Thread.sleep(SLEEPTIME);
                    } catch (InterruptedException e) {
                        continue;
                    }
                }
            }
        }).start();
    }

    private void readAuthKeyFromPref() {
        if (authKey == null) {
            authKey = sp.getString("authKey", null);
            String str = "extra line";
        }
    }

    private void populateListNamesPins(){
        if (namesPins==null || namesPins.isEmpty()) {
            namesPins = new ArrayList<>();
            namesPins.add(Pins.LightSensor);
            namesPins.add(Pins.Presence);
            namesPins.add(Pins.GasSensor);
            namesPins.add(Pins.DoorSensor);
            namesPins.add(Pins.LightActuator);
            namesPins.add(Pins.Alarm);
            namesPins.add(Pins.GasActuator);
        }
        if (conPins==null || conPins.isEmpty()) {
            conPins = new ArrayList<>();
            conPins.add(Pins.ConLight);
            conPins.add(Pins.ConGas);
            conPins.add(Pins.ConDoor);
        }
    }

    private void updConStatus(){

        try {
            for (Pins pin : conPins) {
                urlWriteValueToPin writeConnect = new urlWriteValueToPin();
                writeConnect.execute(pin.value, "0");
                Thread.sleep(3000);
                readJsonFromURL(pin.value);
                JSONArray jsonArray = new JSONArray(json);
                int conInt = Integer.parseInt((String) jsonArray.get(0));
                isCon = (conInt!=0);
                updatePinView(pin);
            }
        } catch (InterruptedException | NumberFormatException | JSONException e) {
            error = "Couldn't connect to check HW conection";
        }
    }

    private void readJsonFromURL(String pinName) {
        urlGetValueOfPin connect = new urlGetValueOfPin();

        try {
            json = connect.execute(pinName).get();
        } catch (Exception e) {
            json = null;
            error = "Error in getting json file from server";
        }
    }

    private void readValueOfPinFromJson(){
        try {
            JSONArray jsonArray = new JSONArray(json);
            valueOfPinFromJson = Double.parseDouble((String)jsonArray.get(0));
        } catch(JSONException e) {
            error = "Error in reading json format";
        }
    }

    private void updatePinView(Pins pin) {
        if (pin.equals(Pins.LightSensor)){
            updLightStatusHandler.sendEmptyMessage(0);
        } else if (pin.equals(Pins.Presence)) {
            updPresenceStatusHandler.sendEmptyMessage(0);
        } else if (pin.equals(Pins.GasSensor)) {
            updGasStatusHandler.sendEmptyMessage(0);
        } else if (pin.equals(Pins.DoorSensor)) {
            updDoorStatusHandler.sendEmptyMessage(0);
        } /*else if (pin.equals(Pins.LightActuator)) {
            updLightActuatorHandler.sendEmptyMessage(0);
        }*/else if (pin.equals(Pins.GasActuator)) {
            updGasActuatorHandler.sendEmptyMessage(0);
        }else if (pin.equals(Pins.Alarm)) {
            updAlarmHandler.sendEmptyMessage(0);
        } else if (pin.equals(Pins.ConLight)) {
            updLightConHandler.sendEmptyMessage(0);
        } else if (pin.equals(Pins.ConGas)) {
            updGasConHandler.sendEmptyMessage(0);
        } else if (pin.equals(Pins.ConDoor)) {
            updDoorConHandler.sendEmptyMessage(0);
        }
    }

    Handler updLightConHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            ImageView lightCon = (ImageView) findViewById(R.id.lightCon);
            if (isCon) {
                lightCon.setImageResource(android.R.drawable.presence_online);
            }
            else {
                lightCon.setImageResource(android.R.drawable.presence_offline);
            }
        }
    };

    Handler updGasConHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            ImageView gasCon = (ImageView) findViewById(R.id.gasCon);
            if (isCon) {
                gasCon.setImageResource(android.R.drawable.presence_online);
            }
            else {
                gasCon.setImageResource(android.R.drawable.presence_offline);
            }
        }
    };

    Handler updDoorConHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            ImageView doorCon = (ImageView) findViewById(R.id.doorCon);
            if (isCon) {
                doorCon.setImageResource(android.R.drawable.presence_online);
            }
            else {
                doorCon.setImageResource(android.R.drawable.presence_offline);
            }
        }
    };

    Handler updLightStatusHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            TextView lightStatus = (TextView) findViewById(R.id.lightStatus);
            if (valueOfPinFromJson >= 50.0) {
                lightStatus.setText("Light is off");
                opposeLightValue = 1;
            }
            else {
                lightStatus.setText("Light is on");
                opposeLightValue = 0;
            }
        }
    };

    Handler updPresenceStatusHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            TextView presenceStatus = (TextView) findViewById(R.id.presenceStatus);

            if (valueOfPinFromJson == 0.0){
                presenceStatus.setText("No presence detected");
            }
            else {
                presenceStatus.setText("Somebody is in the room");
            }
        }
    };

    Handler updGasStatusHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            ProgressBar gasProgressBar = (ProgressBar) findViewById(R.id.gasProgressBar);
            TextView leaking = (TextView) findViewById(R.id.gasLeaking);
            gasProgressBar.setProgress((int) (valueOfPinFromJson * 100) / gasProgressBar.getMax());

            if (valueOfPinFromJson == gasProgressBar.getMax()) {
                leaking.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 18);
                leaking.setText("Possible leakage");
                leaking.setBackgroundColor(0xFFFF0000);
            }
            else {
                double valueToShow = (valueOfPinFromJson * 100) / gasProgressBar.getMax();
                leaking.setTextSize(TypedValue.COMPLEX_UNIT_DIP,13);
                leaking.setText(valueToShow + "% - Normal conditions");
                leaking.setBackgroundColor(Color.TRANSPARENT);
            }
        }
    };

    Handler updDoorStatusHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            TextView doorStatus = (TextView) findViewById(R.id.doorStatus);

            if (valueOfPinFromJson == 0.0) {
                alarmValue = 0;
                doorStatus.setText("Door is open");
                doorStatus.setBackgroundColor(0xFFFF0000);
            }
            else {
                alarmValue = 1;
                doorStatus.setText("Door is closed");
                doorStatus.setBackgroundColor(Color.TRANSPARENT);
            }
        }
    };

    Handler updGasActuatorHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            TextView gasValve = (TextView) findViewById(R.id.gasValve);

            if (valueOfPinFromJson == 0.0) {
                gasValve.setText("Gas Valve is open");
            }
            else {
                gasValve.setText("Gas Valve is closed");
            }
        }
    };

    Handler updAlarmHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            TextView alarmStatus = (TextView) findViewById(R.id.alarmStatus);

            if (valueOfPinFromJson == 0.0) {
                alarmStatus.setText("Alarm not Set");
            }
            else {
                alarmStatus.setText("Alarm set");
            }
        }
    };

    /*Handler updHWConHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            TextView HWCon = (TextView) findViewById(R.id.HWCon);
            HWCon.setText("Sensors are connected");
        }
    };

    Handler updHWUnConHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            TextView HWCon = (TextView) findViewById(R.id.HWCon);
            HWCon.setText("Sensors are not connected, showing last values");
        }
    };*/



    private void populateButtonsWithListeners() {
        Buttons = new ArrayList<>();
        Buttons.add((Button) findViewById(R.id.lightButton));
        Buttons.add((Button) findViewById(R.id.gasButton));
        Buttons.add((Button) findViewById(R.id.doorButton));
        for (Button button : Buttons) {
            final int id = button.getId();
            button.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View view) {
                    sendCmd(id);
                }
            });
        }
    }

    private void sendCmd(int id) {
        error = "";
        try {
            switch (id) {
                case R.id.lightButton:
                    error = new urlWriteValueToPin().execute(Pins.LightActuator.value, ""+opposeLightValue).get();
                    break;
                case R.id.gasButton:
                    error = new urlWriteValueToPin().execute(Pins.GasActuator.value, "1").get();
                    break;
                case R.id.doorButton:
                    error = new urlWriteValueToPin().execute(Pins.Alarm.value, ""+alarmValue).get();
                    break;
                default:
                    error = "Something went terrible wrong";
                    break;
            }
        }catch (InterruptedException | ExecutionException e) {
            error = "thread interrupted";
        }
    }

    /**
     * Get file from URL
     * Has to be async (this was hard)
     */
    class urlGetValueOfPin extends AsyncTask<String,Void,String> {
        @Override
        protected String doInBackground(String... params) {
            try {
                URL url = new URL(SERVERURL + authKey + "/get/" + params[0]);
                InputStreamReader inStream = new InputStreamReader(url.openStream());
                BufferedReader inBuffer = new BufferedReader(inStream);
                try {
                    StringBuilder sb = new StringBuilder();
                    String line;
                    while ((line = inBuffer.readLine()) != null) {
                        sb.append(line);
                    }
                    return sb.toString();
                } catch (IOException e) {
                    return "Error in connecting";
                } finally {
                    if (inBuffer != null) {
                        inBuffer.close();
                        inStream.close();
                    }
                }
            }catch (IOException e) {
                return "Error in closing connection";
            }
        }
    }

    class urlCheckHWCon extends AsyncTask<String,Void,String> {
        @Override
        protected String doInBackground(String... params) {
            try {
                URL url = new URL(SERVERURL + authKey + "/isHardwareConnected");
                InputStreamReader inStream = new InputStreamReader(url.openStream());
                BufferedReader inBuffer = new BufferedReader(inStream);
                try {
                    StringBuilder sb = new StringBuilder();
                    String line;
                    while ((line = inBuffer.readLine()) != null) {
                        sb.append(line);
                    }
                    inBuffer.close();
                    return sb.toString();
                } catch (IOException e) {
                    return "Error in checking HW connection";
                } finally {
                    if (inBuffer != null) {
                        inBuffer.close();
                        inStream.close();
                    }
                }
            } catch (IOException e){
                return "Error in closing HW connection";
            }
        }
    }

    class urlWriteValueToPin extends AsyncTask<String,Void,String> {
        @Override
        protected String doInBackground(String... params) {
            try {
                URL url = new URL(SERVERURL + authKey + "/update/" + params[0] + "?value=" + params[1]);
                InputStreamReader inStream = new InputStreamReader(url.openStream());
                BufferedReader inBuffer = new BufferedReader(inStream);
                try {
                    StringBuilder sb = new StringBuilder();
                    String line;
                    while ((line = inBuffer.readLine()) != null) {
                        sb.append(line);
                    }
                    inBuffer.close();
                    return sb.toString();
                } catch (IOException e) {
                    return "Error in sending command or reading response";
                } finally {
                    if (inBuffer != null) {
                        inBuffer.close();
                        inStream.close();
                    }
                }
            } catch (IOException e){
                return "Error in closing connection";
            }
        }
    }
}
