package com.example.spraveaiandroid;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.os.Bundle;
import android.os.SystemClock;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;

public class BgTask extends BroadcastReceiver {


    @Override
    public void onReceive(Context context, Intent intent) {
        Ringtone ringtone = RingtoneManager.getRingtone(context, RingtoneManager.getDefaultUri(RingtoneManager.TYPE_RINGTONE));
        ringtone.play();
        Toast.makeText(context, "FEF", Toast.LENGTH_LONG).show();

        SystemClock.sleep(1000);
        ringtone.stop();
    }
}
