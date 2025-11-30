package com.ZFFramework.NativeUtil;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ZFTaskId<T> {

    public static final int INVALID = -1;

    private int _taskIdCur = INVALID + 1;
    private final Map<Integer, T> _taskMap = new HashMap<>();

    public synchronized int obtain(T taskData) {
        if (taskData == null) {
            return INVALID;
        }
        do {
            ++_taskIdCur;
            if (_taskIdCur < 0) {
                _taskIdCur = 0;
            }
        } while (_taskMap.containsKey(_taskIdCur));
        _taskMap.put(_taskIdCur, taskData);
        return _taskIdCur;
    }

    public synchronized T release(int taskId) {
        return _taskMap.remove(taskId);
    }

    public synchronized List<T> releaseAll() {
        List<T> ret = new ArrayList<>(_taskMap.values());
        _taskMap.clear();
        return ret;
    }

    public synchronized T exist(int taskId) {
        return _taskMap.get(taskId);
    }

}
