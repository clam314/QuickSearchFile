package filesearch;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import com.clam.filesearch.R;

import java.util.List;

public class MainActivity extends AppCompatActivity {
    TextView tv;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv = (TextView) findViewById(R.id.sample_text);
        query();
    }

    private void query(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                String path = Environment.getExternalStorageDirectory().getAbsolutePath();
                Log.e("FileSearch","java-path:"+path);
//                List<String> list = FileSearch.searchFileBySuffix(path+"/Download",".apk");
                List<String> list = FileSearchHelper.Helper.search(path,".apk");
                if(list != null && !list.isEmpty()){
                    final String show = list.get(0);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            tv.setText(show);
                        }
                    });
                }
            }
        }).start();
    }

}
