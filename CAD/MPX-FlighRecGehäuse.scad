 translate([55,0,0]) union() {
     difference() {
        cube([54,54,5]);
        translate([4.5,2,1.5]) cube([47.5,50,10]);
        translate([25,50,2]) cube([4,10,1.5]);
        translate([2.5,54-32,3]) cylinder(r=1.1, h=10, $fn=100);
        translate([2.5,54-48,3]) cylinder(r=1.1, h=10, $fn=100);
     }
     translate([52,3,4]) cube([1,28,3]);   
     translate([5,52,4]) cube([46,1,3]);   
     translate([5,1,4]) cube([46,1,3]);   
} 

difference() {
  cube([54,54,9]);
  translate([4.5,2,1.5]) cube([47.5,50,10]);
  translate([-1,2.5,6.5]) cube([10,24.5,10]);  
  translate([-1,6.5,-1]) cube([3,15,11]);  
  translate([50,6.5,6.5]) cube([10,14.3,5]);  
  translate([6.5,22,-1]) cylinder(h=10, r=0.75, $fn=100);  

  translate([50,20.5,6]) cube([3,32,13]);
  translate([4.5,1,6]) cube([47.5,3,13]);
  translate([4.5,50,6]) cube([47.5,3,13]);
}  

translate([2.5,32,1]) cylinder(h=10, r=1, $fn=100);
translate([2.5,48,1]) cylinder(h=10, r=1, $fn=100);

