import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.*;

class Student implements Comparable<Student>
{
    int id;
    String name;
    double cgpa;
    public Student(int id,String name,double cgpa)
    {
        this.id=id;
        this.name=name;
        this.cgpa=cgpa;
    }
    public int getID(){return this.id;}
    public String getName(){return this.name;}
    public double getCGPA(){return this.cgpa;}
    @Override
    public int compareTo(Student s)
    {
        if(this.cgpa!=s.getCGPA())
            return s.getCGPA()-this.cgpa>0?1:-1;
        else
        {
            if (this.name.compareTo(s.getName())!=0)
                return this.name.compareTo(s.getName());
            else
                return this.id-s.getID();
        }

      
    }
    public String toString()
    {
        return name;
    }
}
class Priorities
{
    public  List<Student> getStudents(List<String> events)
    {   
        var pq=new PriorityQueue<Student>();
        for(var e:events)
        {
            var words=new Vector<String>();
            var w=new StringBuilder();
            for(int i=0;i<e.length();i++)
                {
                    if(e.charAt(i) !=' ')
                        w.append(e.charAt((i)));
                    else
                    {   
                      
                        words.add(w.toString());
                        w=new StringBuilder();
                    
                    }
                }
            words.add(w.toString());
                       
             
            if(words.get(0).compareTo("SERVED")==0)
            {
                pq.poll();        
            }else
            {
                var s=new Student(Integer.parseInt(words.get(3)),words.get(1),Double.parseDouble(words.get(2)));
                pq.add(s);
            }        
        }
        var ans=new ArrayList<Student>();
        while(!pq.isEmpty())
            ans.add(pq.poll());
        return ans;
        }
}

public class Solution {
    private final static Scanner scan = new Scanner(System.in);
    private final static Priorities priorities = new Priorities();
    
    public static void main(String[] args) {
        int totalEvents = Integer.parseInt(scan.nextLine());    
        List<String> events = new ArrayList<>();
        
        while (totalEvents-- != 0) {
            String event = scan.nextLine();
            events.add(event);
        }
        
        List<Student> students = priorities.getStudents(events);
        
        if (students.isEmpty()) {
            System.out.println("EMPTY");
        } else {
            for (Student st: students) {
                System.out.println(st.getName());
            }
        }
    }
}