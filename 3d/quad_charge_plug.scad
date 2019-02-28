

use <lib/cube.scad>;
module plug_surface () {
    projection()
    translate([0,0,10])
    intersection()
    {
        plug();
        translate([-10,-10,-10])
        ;//cube([20,20,10]);
    }
}
module plug() {
    
        rotate([-90,0,0])
            import("quad_plug.stl");
}

translate([0,0,-10+10*$t])
rotate([180,0,180])
%plug() ;
difference () {
    aligned_cube([8,5,5]);
    union() {
        translate([0,0,-0.1])
    scale([1.05,1.1,1])
        linear_extrude(4)
            plug_surface();
    }

    // pin cuts
    for (i= [1,-1]) {
        translate([1*i,0,0])
        aligned_cube([1,1,50]); 
        translate([1*i,0,-1])
        aligned_cube([2,2,5]); 
    }
}