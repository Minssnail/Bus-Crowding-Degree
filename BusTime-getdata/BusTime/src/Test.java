import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import org.apache.http.HttpResponse;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.concurrent.FutureCallback;
import org.apache.http.impl.nio.client.CloseableHttpAsyncClient;
import org.apache.http.impl.nio.client.HttpAsyncClients;
import org.json.JSONException;

import java.net.URLEncoder;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.time.LocalDateTime;
import java.util.regex.Pattern;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;
import java.util.concurrent.CountDownLatch;
import java.util.regex.Matcher;

public class Test {
	static BufferedReader bfr;
	public static void main(String[] args) throws Exception {
		
		RequestConfig config = RequestConfig.custom().setSocketTimeout(10000).setConnectTimeout(10000).build();
		CloseableHttpAsyncClient httpclient = HttpAsyncClients.custom().setDefaultRequestConfig(config).build();
		httpclient.start();
		
		Pattern pattern = Pattern.compile("(\\w+)_(\\d):([\\u4e00-\\u9fa5|\\w|¡¢]+)");
		Matcher matcher = pattern.matcher("");
		bfr = new BufferedReader(new FileReader("C:\\Users\\lenovo\\Desktop\\busline2.txt"));
		
		TreeMap<String,HttpGet> requests = new TreeMap<String, HttpGet>();
		HashMap<String,GetBusTime> processors = new HashMap<String, GetBusTime>();
		HashMap<String,BufferedWriter> writers = new HashMap<String, BufferedWriter>();

		String line;
		String lineName;
		String fromStation;
		String url;
		int direction;
		
		Class.forName("com.mysql.cj.jdbc.Driver");
		Connection conn = DriverManager.getConnection("jdbc:mysql://127.0.0.1:3306/Bus?characterEncoding=UTF-8&useSSL=true&serverTimezone=UTC", "root", "6w0899.");
		System.out.println("connected");
		
		while((line = bfr.readLine()) != null) {
			matcher.reset(line);
			while(matcher.find()) {
				lineName = matcher.group(1);
				direction = Integer.parseInt(matcher.group(2));
				fromStation = matcher.group(3);
				File file = new File("C:\\Users\\lenovo\\Desktop\\GetData\\"+lineName+"_"+direction+".txt");
				file.createNewFile();
				url = "http://www.zhbuswx.com/Handlers/BusQuery.ashx?handlerName=GetBusListOnRoad&lineName="
						+ lineName + "%E8%B7%AF&fromStation=" + URLEncoder.encode(fromStation,"utf-8");
				HttpGet get = new HttpGet(url);
				get.setHeader("Host", "www.zhbuswx.com");
				get.setHeader("Connection", "keep-alive");
				get.setHeader("Accept", "application/json, text/javascript, */*; q=0.01");
				get.setHeader("X-Requested-With", "XMLHttpRequest");
				get.setHeader("Referer", "http://www.zhbuswx.com/busline/BusQuery.html?v=2.01");
				get.setHeader("User-Agent", "Mozilla/4.0 (compatible; MSIE 5.0; Windows NT; DigExt)");
				get.setHeader("Cookie", "openid3=hrgenbsOdb1V7AihzW7RD2gHPQPW");
				requests.put(lineName+"_"+direction,get);
				writers.put(lineName+"_"+direction,new BufferedWriter(new FileWriter(file)));
				processors.put(lineName+"_"+direction, new GetBusTime(lineName+"_"+direction));
			}
		}
		
		Set<Entry<String,HttpGet>> entrySet = requests.entrySet();
		
		final CountDownLatch latch = new CountDownLatch(9999999);
		while(true) {

			for(final Entry<String,HttpGet> entry : entrySet) {
				
				httpclient.execute(entry.getValue(), new FutureCallback<HttpResponse>() {
				
					@Override
					public void cancelled() {
						// TODO Auto-generated method stub
						latch.countDown();
						System.out.println("----");
					}
	
					@Override
					public void completed(HttpResponse res) {
						// TODO Auto-generated method stub
						latch.countDown();
						LocalDateTime time = LocalDateTime.now();
						try {
							String line;
							BufferedReader buf = new BufferedReader(new InputStreamReader(res.getEntity().getContent(),"utf-8"));
							if((line = buf.readLine()) != null){

								writers.get(entry.getKey()).write(time+System.getProperty("line.separator")+line+System.getProperty("line.separator")+"-------------------------"+System.getProperty("line.separator"));
								writers.get(entry.getKey()).flush();
								processors.get(entry.getKey()).process(line, time, conn);
							}
						} catch (UnsupportedOperationException e) {
							// TODO Auto-generated catch block
							System.out.println("UnsupportedOperationException");
						} catch (IOException e) {
							// TODO Auto-generated catch block
							System.out.println("IOException");
						} catch(ClassNotFoundException e) {
							System.out.println("ClassNotFoundException");
						} catch(SQLException e) {
							System.out.println("SQL Exception");
						} catch(JSONException e) {
							System.out.println("Response Error");
						}
					}
	
					@Override
					public void failed(Exception e) {
						// TODO Auto-generated method stub
						latch.countDown();
						System.out.println("HTTP Failed");
					}
					
				});
				
				Thread.sleep(555);
			}
		}
	
	}
	

}
