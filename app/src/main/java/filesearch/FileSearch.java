package filesearch;

import java.util.List;

/**
 * 创建时间： 2017/8/14.
 * 编写人：wuweimin
 * 功能描述：
 */

public class FileSearch {
    public static native List<String> searchFileBySuffix(String rootPath, String suffix);

    static {
        System.loadLibrary("FileSearch");
    }
}