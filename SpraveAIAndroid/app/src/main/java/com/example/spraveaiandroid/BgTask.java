package com.example.spraveaiandroid;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.os.SystemClock;
import android.widget.Toast;

public class BgTask extends BroadcastReceiver {


    @Override
    public void onReceive(Context context, Intent intent) {
        Ringtone ringtone = RingtoneManager.getRingtone(context, RingtoneManager.getDefaultUri(RingtoneManager.TYPE_RINGTONE));
        while(true)
        {
            ringtone.play();
            Toast.makeText(context, "FEF", Toast.LENGTH_LONG).show();
            SystemClock.sleep(1000);
            ringtone.stop();
            SystemClock.sleep(1000);
        }

    }
}
