package labsoft.homeelderaid;


import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.TargetApi;
import android.content.pm.PackageManager;
import android.support.annotation.NonNull;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.app.LoaderManager.LoaderCallbacks;

import android.content.CursorLoader;
import android.content.Loader;
import android.database.Cursor;
import android.net.Uri;
import android.os.AsyncTask;

import android.os.Build;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.text.TextUtils;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.EditorInfo;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.ToggleButton;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by Luiz on 04/11/2016.
 */

public class MainActivity extends AppCompatActivity {

    private String authKey;
    private String json;
    private String error;
    private List<String> namesPins;

    private ToggleButton toggleButton;

    private double valueOfPinFromJson;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_principal);

        readAuthKeyFromFile();

        populateListNomesPinos();

        for (String pin : namesPins) {
            readJsonFromURL(pin);
            readValueOfPinFromJson();
            updatePinView(pin);
            json = null;
        }
    }

    private void readAuthKeyFromFile() {
        if (authKey == null) {
            try {
                InputStreamReader inputStreamReader = new InputStreamReader(MainActivity.this.openFileInput("authKey.txt"));
                BufferedReader reader = new BufferedReader(inputStreamReader);
                authKey = reader.readLine();
            } catch (IOException e) {
                authKey = null;
                error = "Error in getting authentication Key";
            }
        }
    }

    private void populateListNomesPinos(){
        if (namesPins==null || namesPins.isEmpty()){
            namesPins = new ArrayList<>();
            namesPins.add("V5");
            namesPins.add("");
            namesPins.add("");
            namesPins.add("");
        }
    }

    private void readJsonFromURL(String pin) {
        if (json == null) {
            urlConnect connect = new urlConnect();

            try {
                json = connect.execute(pin).get();
            } catch (Exception e) {
                json = null;
                error = "Error in getting json file from server";
            }
        }
    }

    private void readValueOfPinFromJson(){
        try {
            JSONArray jsonArray = new JSONArray(json);
            valueOfPinFromJson = (Double)jsonArray.get(0);
        } catch(JSONException e) {
            error = "Error in reading json format";
        }
    }

    private void updatePinView(String pin) {
        /*switch (pin) {
            case "":
                toggleButton = (ToggleButton) findViewById(R.id.lightButton);
                toggleButton.
                break;
        }*/
    }

    /**
     * Get file from URL
     * Has to be async (this was hard)
     */
    class urlConnect extends AsyncTask<String,Void,String> {
        @Override
        protected String doInBackground(String... params) {
            try {
                StringBuilder sb = new StringBuilder();
                URL url = new URL("http://blynk-cloud.com/"+ authKey + "/get/" + params[0]);
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
}
