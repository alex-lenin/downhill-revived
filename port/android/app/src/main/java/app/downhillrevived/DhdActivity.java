// Downhill Revived on Android (finding 091): SDL's activity, which loads libSDL3.so and libmain.so
// (the game, port/tools/build_host64.py --target android-arm64) and runs SDL_main (port/host/android_main.c) on its
// own thread. Here only what the game asks of the activity before that: the screen's shape.
package app.downhillrevived;

import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;

import org.libsdl.app.SDLActivity;

public class DhdActivity extends SDLActivity {
    @Override
    protected String[] getLibraries() {
        return new String[] { "SDL3", "main" };
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // The screen's shape (its long side over its short one), for widescreen = 1:
        // host_monitor_aspect reads DHD_MONITOR_ASPECT. Set before SDL_main starts.
        try {
            Rect bounds = getWindowManager().getMaximumWindowMetrics().getBounds();
            int w = Math.max(bounds.width(), bounds.height()), h = Math.min(bounds.width(), bounds.height());
            if (h > 0) nativeSetenv("DHD_MONITOR_ASPECT", Double.toString((double) w / h));
        } catch (Throwable e) {  // the libraries didn't load: SDLActivity shows why
            Log.w("DownhillRevived", "no screen shape: " + e);
        }
    }
}
