package labsoft.homeelderaid;

import android.support.v7.app.AppCompatActivity;

import android.os.AsyncTask;

import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
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
    private String authKey;

    private String json;
    private String error;
    private List<Pins> namesPins;

    private List<Button> Buttons;
    //private Button lightButton, gasButton, doorButton;

    private double valueOfPinFromJson;
    private int opposeLightValue, alarmValue;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_principal);

        readAuthKeyFromFile();

        populateListNomesPinos();

        populateButtonsWithListeners();

        while (true) {
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

    private void readAuthKeyFromFile() {
        if (authKey == null) {
            try {
                InputStreamReader inputStreamReader = new InputStreamReader(MainActivity.this.openFileInput("authKey.txt"));
                BufferedReader reader = new BufferedReader(inputStreamReader);
                authKey = reader.readLine();
                inputStreamReader.close();
                reader.close();
            } catch (IOException e) {
                authKey = null;
                error = "Error in getting authentication Key";
            }
        }
    }

    private void populateListNomesPinos(){
        if (namesPins==null || namesPins.isEmpty()){
            namesPins = new ArrayList<>();
            namesPins.add(Pins.Light);
            namesPins.add(Pins.Presence);
            namesPins.add(Pins.Gas);
            namesPins.add(Pins.Door);
        }
    }

    private void readJsonFromURL(String pinName) {
        if (json == null) {
            urlGetValueOfPin connect = new urlGetValueOfPin();

            try {
                json = connect.execute(pinName).get();
            } catch (Exception e) {
                json = null;
                error = "Error in getting json file from server";
            }
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
        if (pin.equals(Pins.Light)){
            updLightStatus();
        } else if (pin.equals(Pins.Presence)) {
            updPresenceStatus();
        } else if (pin.equals(Pins.Gas)) {
            updGasStatus();
        } else if (pin.equals(Pins.Door)) {
            updDoorStatus();
        }
    }

    private void updLightStatus() {
        TextView lightStatus = (TextView) findViewById(R.id.lightStatus);
        if (valueOfPinFromJson == 0.0) {
            lightStatus.setText("Light is off");
            opposeLightValue = 1;
        }
        else {
            lightStatus.setText("Light is on");
            opposeLightValue = 0;
        }
    }

    private void updPresenceStatus() {
        //TODO create and update how to show presence
    }

    private void updGasStatus() {
        ProgressBar gasProgressBar = (ProgressBar) findViewById(R.id.gasProgressBar);
        TextView leaking = (TextView) findViewById(R.id.gasLeaking);
        gasProgressBar.setProgress((int) valueOfPinFromJson);

        if (valueOfPinFromJson == gasProgressBar.getMax()) {
            leaking.setText("Possible leakage");
            leaking.setBackgroundColor(0xFFFF0000);
        }
        else {
            leaking.setText("Normal conditions");
            leaking.setBackgroundColor(0xFFD3D3D3);
        }
    }

    private void updDoorStatus() {
        TextView doorStatus = (TextView) findViewById(R.id.doorStatus);
        alarmValue = (int) valueOfPinFromJson;

        if (valueOfPinFromJson == 0.0) {
            doorStatus.setText("Door is open");
            doorStatus.setBackgroundColor(0xFFFF0000);
        }
        else {
            doorStatus.setText("Door is closed");
            doorStatus.setBackgroundColor(0xFFD3D3D3);
        }
    }

    private void populateButtonsWithListeners() {
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
                    error = new urlWriteValueToPin().execute(Pins.Light.value, ""+opposeLightValue).get();
                    break;
                case R.id.gasButton:
                    error = new urlWriteValueToPin().execute(Pins.LedGas.value, "1").get();
                    break;
                case R.id.doorButton:
                    error = new urlWriteValueToPin().execute(Pins.LedAlarm.value, ""+alarmValue).get();
                    break;
                default:
                    error = "Something went terrible wrong";
                    break;
            }
        }catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
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
                StringBuilder sb = new StringBuilder();
                URL url = new URL(SERVERURL + authKey + "/get/" + params[0]);
                BufferedReader in = new BufferedReader(new InputStreamReader(url.openStream()));
                String line;
                while ((line = in.readLine()) != null) {
                    sb.append(line);
                }
                return sb.toString();
            }catch (Exception e) {
                e.printStackTrace();
                return "Error in connecting";
            }
        }
    }

    class urlWriteValueToPin extends AsyncTask<String,Void,String> {
        @Override
        protected String doInBackground(String... params) {
            try {
                StringBuilder sb = new StringBuilder();
                URL url = new URL(SERVERURL + authKey + "/update/" + params[0] + "?value=" + params[1]);
                BufferedReader in = new BufferedReader(new InputStreamReader(url.openStream()));
                String line;
                while ((line = in.readLine()) != null) {
                    sb.append(line);
                }
                return sb.toString();
            }catch (MalformedURLException e) {
                e.printStackTrace();
                return "MalFormed URL";
            }catch (IOException e){
                e.printStackTrace();
                return "Error in sending command or reading reesponse";
            }
        }
    }
}
