package com.example.sust_day01;

import org.xutils.x;

import android.app.Application;

public class MyApplication extends Application{
	@Override
	public void onCreate() {
		super.onCreate();
		//��XUtls���г�ʼ��
		x.Ext.init(this);
	}
}
