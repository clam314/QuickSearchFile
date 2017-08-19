package filesearch;

import android.util.Log;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * 创建时间： 2017/8/19.
 * 编写人：wuweimin
 * 功能描述：
 */

public enum FileSearchHelper {
    Helper;

    private static final String TAG = FileSearchHelper.class.getSimpleName();
    CountDownLatch latch;


    public List<String> search(String rootPath, final String suffix) {
        final List<String> apkPath = Collections.synchronizedList(new ArrayList<String>());
        List<String> directoryPath = new ArrayList<>(80);
        File[] root = new File(rootPath).listFiles();
        if(root == null) return apkPath;
        for(File file: root){
            if(file.isDirectory()){
                directoryPath.add(file.getAbsolutePath());
            }else if(file.isFile() && file.getPath().endsWith(suffix)){
                apkPath.add(file.getAbsolutePath());
            }
        }
        latch = new CountDownLatch(directoryPath.size());
        ExecutorService service = Executors.newFixedThreadPool(10);
        for(final String path : directoryPath){
            service.execute(new Runnable() {
                @Override
                public void run() {
                    Log.e(TAG,"Thread run: "+ Thread.currentThread().getName()+"path:"+path);
                    apkPath.addAll(FileSearch.searchFileBySuffix(path,suffix));
                    latch.countDown();
                }
            });
        }
        try {
            latch.await();
        }catch (InterruptedException e){
            e.printStackTrace();
        }
        return apkPath;
    }
}
