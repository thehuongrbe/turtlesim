#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <turtlesim/Spawn.h>
using namespace std;
const float PI = 3.14159265;
float rate = 5;
geometry_msgs::Twist getMessage(double linear_x, double angular_z)
{
    geometry_msgs::Twist msg;
    msg.linear.x = linear_x;
    msg.angular.z = angular_z;
    return msg;
}
class PoseCallback {
public:
    int turtle_idx;
    ros::Subscriber sub;
    turtlesim::Pose current_pose;
    void callback(const turtlesim::Pose::ConstPtr& msg)
    {
        cout << "turtle " << turtle_idx+1 << " " << msg->x << " " << msg->y << endl;
        current_pose = *msg;
    }
};
int distancemin(PoseCallback &sub,int n,char **argv,int mang[],int n_mang){
    double x0 = atof(argv[mang[0]]), y0 = atof(argv[mang[0]+1]);
    double distancemin=sqrt( pow(x0-sub.current_pose.x, 2) + pow(y0-sub.current_pose.y, 2) );
    int min=mang[0];
    for(int i=0;i<n_mang;i++){
        double x0 = atof(argv[mang[i]]), y0 = atof(argv[mang[i]]);
        double distance=sqrt( pow(x0-sub.current_pose.x, 2) + pow(y0-sub.current_pose.y, 2) );
        if(distance<distancemin){
            distancemin=distance;
            min=mang[i];
        }
        
    }
    cout<<"vi tri min"<<min<<endl;
    return min;
}
void xoamang(int a[],int &n,int x){
    cout<<"xoa mang ok"<<endl;
if(x==a[n-1]){n--;}
else{
    for(int i=0;i<n-1;i++){
        if(a[i]==x){
            for(int j=i;j<n-1;j++){
                a[j]=a[j+1];
            }
                n--;
        }
    }
}
a[n];
}

    void runturtle(ros::Publisher pub[],PoseCallback sub[],int argc,char **argv,int ma[],int &n,int n_turtle){
        const double tolerance = 1e-2+0.001;
        int mangmin[n_turtle];
        ros::Rate loopRate(rate);     
        int dem=1;        
            while(n>0){
                int sorua;
                 bool ruadendich[n_turtle];
                 if(dem==1){ 
                      while(ros::ok()){
                          loopRate.sleep();
                 pub[0].publish(getMessage(0,0));
                     ros::spinOnce(); //cap nhat toa do rua 1
                         cout<<sub[0].current_pose.x<<endl;
                       break;
                      }
                 } 
                 dem++;
                if(n>=n_turtle){      
                for (int idx = 0; idx <n_turtle; idx++){
                 int a=distancemin(sub[idx],argc,argv,ma,n);
                 xoamang(ma,n,a);
                 mangmin[idx]=a;
                 ruadendich[idx]=false;
                }
                sorua=n_turtle;//so rua chay vong lap;
                }
                else{      
                     sorua=n;
                     cout<<"so rua la"<<sorua<<endl;
                for (int idx = 0;idx <sorua;idx++){
                 int a=distancemin(sub[idx],argc,argv,ma,n);
                 xoamang(ma,n,a);
                 mangmin[idx]=a;
                 ruadendich[idx]=false;
                }
                }
                 while(ros::ok()){
                    loopRate.sleep(); 
                     ros::spinOnce();            
             for (int idx = 0; idx <sorua; idx++){  
                double x0=atof(argv[mangmin[idx]]);
                double y0=atof(argv[mangmin[idx]+1]);         
                bool test=true;//dieu khien tien lui
                geometry_msgs::Twist msg;
               
               double distance=sqrt( pow(x0-sub[idx].current_pose.x, 2) + pow(y0-sub[idx].current_pose.y, 2) );
            cout<<"ok"<<endl;
            if (distance < tolerance) { // dừng robot khi đến đủ gần
                pub[idx].publish(getMessage(0,0));
		        test=true;
                ruadendich[idx]=true;
            }
            double alpha = atan2( y0-sub[idx].current_pose.y, x0-sub[idx].current_pose.x ),a_z;
            double dx = x0 - sub[idx].current_pose.x, dy = y0 - sub[idx].current_pose.y, theta = sub[idx].current_pose.theta;
            double dalpha = asin ((cos(theta)*dy-sin(theta)*dx) / distance);
            
         if(alpha>=sub[idx].current_pose.theta+PI-1.5&&alpha<=sub[idx].current_pose.theta+PI+1.5&&test==true){	
             msg = getMessage(-min(5*distance,6.5),-6*dalpha);
              pub[idx].publish(msg);      
            	}
                /* code */
else{
           msg = getMessage(min(4*distance,6.5),6.5*dalpha);
		test=false;  
        pub[idx].publish(msg);     
}                    
             }
                int kt=0;
                for(int i=0;i<sorua;i++){//
                    if(ruadendich[i]==true) kt++;
                }
                if(kt==sorua)
                break;       
      }
      }
    }
int main(int argc, char** argv)
{
    ros::init(argc, argv, "myturtle_control");
    ros::NodeHandle h;
    ros::Publisher pub[10];
    PoseCallback sub[10];
int sopt=(argc-1)/2;
        int mang[sopt];//mang luu vi tri cac toa do trong argv;
        for(int i=0;i<sopt;i++){
           mang[i]=2*i+2;
        }
        int n_turtle = atoi(argv[1]);
        for(int i = 1 ; i < n_turtle ; i++)
    {
        ros::service::waitForService("spawn");
        ros::ServiceClient spawner = h.serviceClient<turtlesim::Spawn>("spawn");
        turtlesim::Spawn turtle;
        turtle.request.x = (rand() % 22)/2;
        turtle.request.y = (rand() % 22)/2;
        turtle.request.theta = (rand() %3);
        spawner.call(turtle);
    }
    
    //cout << "n_turtle = " << n_turtle << endl;
    for (int i = 0; i < n_turtle; i++) {
        stringstream s;
        s << "turtle" << i+1;
        string name = s.str();
        pub[i] = h.advertise<geometry_msgs::Twist>(name + "/cmd_vel", 1000);
        sub[i].turtle_idx = i;
        sub[i].sub = h.subscribe(name+"/pose", 1000, &PoseCallback::callback, &sub[i]);
        //cout << "subcribe turtle " << i << " to " << name << "/pose" << endl;
    }
    
    ros::Rate loopRate(rate);
             const double tolerance = 1e-2;
           runturtle(pub,sub,argc,argv,mang,sopt,n_turtle);//chay den distance min;
           if(sopt==0)cout<<"end"<<endl;
    return 0;
}


