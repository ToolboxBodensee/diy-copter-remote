//parametric micro quadcopter frame for lulfro and others
//Patrick Sapinski
//v2
//01/06/16
//https://www.thingiverse.com/thing:843597

motorD = 9;
motorZ = 22;
shellThickness = 3.5;
armW = 6;
m2mDist = 100;
batteryWidth = 35.0;
quadThickness = 5;

usbWidth = 12;
usbOff = 0;

propLength = 70;

module makeArmHollow() {
    translate([m2mDist/2,0,0]) 
        union(){
            
        //hollow out the motor holder
        sphere(r=motorD/2);
        cylinder(h = motorZ/2, r=motorD/2);
        
        //hollow out the groove for the wire in the motor holder
        translate([-shellThickness,0,-shellThickness]) 
            cube([m2mDist,armW - shellThickness,motorZ],center=true);
        
        //???
        translate([-shellThickness*2,-armW/2 + shellThickness/2,-motorZ/2]) 
            cube([motorZ,armW - shellThickness,motorZ + shellThickness]);
            
    }
}

difference() {
    union(){
        
        //translate([15,5,4]) rotate([90,0,0]) linear_extrude(height = 2) polygon(points=[[0,0],[10,0],[12,7],[0,7]]);
        //translate([15,-3,4]) rotate([90,0,0]) linear_extrude(height = 2) polygon(points=[[0,0],[10,0],[12,7],[0,7]]);
        
        //create each arm
        rotate([0,0,45]){
        for (i = [0 : 3])
            rotate([0,0,i * 90])
            translate([m2mDist/2,0,0]) 
            //create the motor holder and arm
            union(){
                    translate([-m2mDist/2 + motorD/2,-armW/2,motorZ/2 - armW]) 
                        cube([m2mDist/2,armW,armW]);
                cylinder(h = motorZ/2, r=motorD/2 + shellThickness/2);
                sphere(r=motorD/2 + shellThickness/2);
                
                //prop preview
                //%rotate([0,0,45]) cylinder(d=propLength, h=5);
                
                // print helper discs ("brim")
                translate([0, 0, 10.8])
                    cylinder(h=0.2, d=40);
            }
        }
        
        //create the FC cube
        translate([0,0,motorZ/2 - armW/2]) 
            cube([batteryWidth + shellThickness,batteryWidth + shellThickness,armW],center=true);
        
        //parabolic arms
        translate([0,0,8]) 
            difference() {
                cube([m2mDist-20,m2mDist-20,armW],center=true);
                    for (i = [0 : 3])
                        rotate([0,0,i * 90])
                            translate([m2mDist - 35,0,-10]) 
                                oval(m2mDist/2,m2mDist/2.8, 20);
            }
    }
    
    translate([usbOff, batteryWidth / 2 - 2, -1])
        cube([usbWidth, 5, 10]);
    
    //create each arms hollow area
    rotate([0,0,45]){
            rotate([0,0,90]) makeArmHollow();
            rotate([0,0,180]) makeArmHollow();
            rotate([0,0,270]) makeArmHollow();
            rotate([0,0,360]) makeArmHollow();
    }
    
    translate([0,0,motorZ/2 - armW/2 - motorZ/2 + shellThickness/2])
    union(){
        //hollow out some grooves in the arms for the wires
        rotate([0,0,45])
            cube([armW - shellThickness,m2mDist/2,motorZ/1.5],center=true);
        rotate([0,0,45])
            cube([m2mDist/2,armW - shellThickness,motorZ/1.5],center=true);
    }
    
    //hollow out the FC hole
    translate([0,0,motorZ/2 - armW/2 - shellThickness/2]) 
    cube([batteryWidth,batteryWidth,armW],center=true);
    
    //drunk code below
    b = 11;
    h = 11;
    w = 4;
    rotate(a=[0,0,45])
        translate([2,2,motorZ/2]) 
            linear_extrude(height = w, center = true, convexity = 10, twist = 0)
                polygon(points=[[0,0],[h,0],[0,b]], paths=[[0,1,2]]);
        
    rotate(a=[0,0,45 + 180])
        translate([2,2,motorZ/2]) 
            linear_extrude(height = w, center = true, convexity = 10, twist = 0)
                polygon(points=[[0,0],[h,0],[0,b]], paths=[[0,1,2]]);
        
    rotate(a=[0,0,45 + 90])
        translate([2,2,motorZ/2]) 
            linear_extrude(height = w, center = true, convexity = 10, twist = 0)
                polygon(points=[[0,0],[h,0],[0,b]], paths=[[0,1,2]]);
        
    rotate(a=[0,0,45 + 270])
        translate([2,2,motorZ/2]) 
            linear_extrude(height = w, center = true, convexity = 10, twist = 0)
                polygon(points=[[0,0],[h,0],[0,b]], paths=[[0,1,2]]);
                
    
}

module oval(w,h, height, center = false) {
    scale([1, h/w, 1]) cylinder(h=height, r=w, center=center);
}



