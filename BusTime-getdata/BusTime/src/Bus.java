import java.sql.Connection;
import java.sql.Date;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.time.Duration;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Collections;

public class Bus {
	private int LastPosition;
	private String line;
	private int stationNum;
	private ArrayList<LocalDateTime> list;
	private ArrayList<LocalDateTime> list2;
	Connection conn;
	private static int listsCount = 0;

	public Bus(String line,int stationNum,int LastPosition,LocalDateTime time,Connection conn) {
		this.stationNum = stationNum;
		this.LastPosition = LastPosition;
		this.line = line;
		this.conn = conn;
		list = new ArrayList<LocalDateTime>(Collections.nCopies(Constants.map.get(line).size(), null));
		list2 = new ArrayList<LocalDateTime>(Collections.nCopies(Constants.map.get(line).size(), null));
		if(stationNum == 0)
			setTime(0,time);
	}
	
	
	public int getStationNum() {
		return stationNum;
	}


	public void setStationNum(int stationNum) {
		this.stationNum = stationNum;
	}


	public int getLastPosition() {
		return LastPosition;
	}
	
	
	public void setLastPosition(int LastPosition) {
		this.LastPosition = LastPosition;
	}
	
	
	public boolean isEndEmpty() {
		if(list.get(list.size()-1) == null)
			return true;
		return false;
	}
	
	
	public void setTime(int n,LocalDateTime time) throws ArrayIndexOutOfBoundsException{
		list.set(n, time);
	}
	
	
	public void setTime2(int n,LocalDateTime time) throws ArrayIndexOutOfBoundsException{
		list2.set(n, time);
	}
	

	private int timeDiffer(int n) throws ArrayIndexOutOfBoundsException{
		if(this.list.get(n) == null || this.list.get(n+1) == null)
			return -1;
		Duration duration = Duration.between(this.list.get(n), this.list.get(n+1));
		return (int)duration.getSeconds();
	}
	
	private int timeDiffer2(int n) {
		if(this.list.get(n) == null || this.list2.get(n) == null || n == 0 || n == list.size()-1)
			return -1;
		Duration duration = Duration.between(this.list.get(n), this.list2.get(n));
		return (int)duration.getSeconds();
	}

	public void saveData(String url, String user, String password) throws SQLException, ClassNotFoundException{
		
		PreparedStatement ps = conn.prepareStatement("INSERT INTO "+line+"(`Date`,`Hour`,`Station`,`Stop`,`Duration`) VALUES(?,?,?,?,?)");
		System.out.println(list.toString());
		System.out.println(list2.toString());
		
		for(int i = 0,t = 0; i < Constants.map.get(line).size()-1; i++) {
			
			if((t = timeDiffer(i)) > 0) {
				ps.setDate(1, Date.valueOf(list.get(i).toLocalDate()));
				ps.setInt(2, list.get(i).getHour());
				ps.setInt(3, i);
				ps.setInt(4, timeDiffer2(i));
				ps.setInt(5, t);
				ps.addBatch();
			}
			System.out.print(t+" ");
			
		}
		System.out.println();
		for(int i = 0; i < Constants.map.get(line).size()-1; i++) {
			System.out.print(timeDiffer2(i)+" ");
		}
		int[] res = ps.executeBatch();
		listsCount += res.length;
		System.out.println();
		System.out.println(res.length+"("+listsCount+") Lists Added Successfully");
		ps.clearParameters();
	}
	
}
