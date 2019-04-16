import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
import java.sql.SQLException;
import java.time.LocalDateTime;
import java.util.HashMap;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import java.util.Map;
import java.util.Set;
import java.util.HashSet;
import java.util.Properties;

public class GetBusTime {
	
	static int count = 0;
	int total;
	Set<String> busNumberList;
	Set<String> tempBusList = new HashSet<String>();
	Map<String,Bus> buses = new HashMap<String,Bus>();
	Properties prop = new Properties();
	String line;
	
	public GetBusTime(String line) {
		this.line = line;
		busNumberList = new HashSet<String>();
		total = Constants.map.get(line).size();
		try {
			prop.load(new FileReader("in.properties"));
		}catch(IOException e) {
			e.printStackTrace();
		}
	}
		
		
//////////////////////////////////////////////////////////////////////		
	public void process(String res, LocalDateTime time, Connection conn) throws ClassNotFoundException, SQLException, JSONException{
		String busNumber;
		int stationNum;
		int lastPosition;
			JSONObject jobj = new JSONObject(res);

		switch(jobj.getInt("flag")) {
			case 1002:
				JSONArray arr = jobj.getJSONArray("data");
				for(int i = 0; i < arr.length(); i++) {
					JSONObject obj = arr.getJSONObject(i);
					busNumber = obj.getString("BusNumber");
					stationNum = Constants.map.get(line).indexOf(obj.getString("CurrentStation"));
					lastPosition = obj.getInt("LastPosition");
					tempBusList.add(busNumber);
					
					if(!buses.containsKey(busNumber)) {
						Bus tempbus = new Bus(line,stationNum,lastPosition,time,conn);
						if(stationNum == 0)
							tempbus.setTime(0, time);
						buses.put(busNumber,tempbus);
						
					}
					else {
						Bus bus = buses.get(busNumber);
						if(stationNum == bus.getStationNum() + 1) {
							bus.setStationNum(stationNum);
							bus.setTime(stationNum, time);
							if(lastPosition == 5) 
								bus.setLastPosition(5);
						}else if(stationNum == bus.getStationNum()) {
							if(lastPosition == bus.getLastPosition()+3) {
								bus.setTime2(bus.getStationNum(), time);
								bus.setLastPosition(lastPosition);
							}
						}else {
							System.out.println("Êý¾ÝÖÐ¶Ï"+":"+line+"\t"+time+"\t"+busNumber+"\t");
							bus.setLastPosition(lastPosition);
							bus.setStationNum(stationNum);
						}
					}
				}
				count++;
				break;
				
			default:
				return;
		}

		busNumberList.removeAll(tempBusList);

		if(!busNumberList.isEmpty()) {
			System.out.println();
			System.out.println("Removed:"+busNumberList.toString()+"\t"+line+"\t"+time);
			for(String str : busNumberList) {
				Bus bus = buses.get(str);
				if(bus.isEndEmpty()) {
					bus.setTime(total-1, time);
				}
				bus.saveData(prop.getProperty("url"),prop.getProperty("user"),prop.getProperty("password"));
				buses.remove(str);
			}
			busNumberList.clear();
		}

		busNumberList.addAll(tempBusList);
		tempBusList.clear();
		
		if(count%100 == 0) {
			System.out.println();
			System.out.println("Avaliable Data Count:"+count/100+" Hundreds");
			System.out.println();
		}
	}
//////////////////////////////////////////////////////////////////////////////	
	

		
}