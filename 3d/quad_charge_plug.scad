

use <lib/cube.scad>;
module plug_surface () {
    projection()
    translate([0,0,10])
    {
        plug();
    }
}
module plug() {
        rotate([270,0,0])
            import("quad_plug.stl");
}

module charger_plug() {
    difference () {
        // hull
        aligned_cube([9,6,5]);
        
        // plug cut
        translate([0,0,1])
        union() {
            scale([1.20,1.4,1])
            linear_extrude(5)
                plug_surface();
        }

        // pin cuts
        for (i= [1,-1]) {
            translate([1*i,-0.2,-0.01])
                aligned_cube([0.99,0.99,50]); 
            translate([1*i,0,1])
                aligned_cube([2,2,5]); 
        }
    }
    translate([0,1.9,-0.01])
        aligned_cube([9,2,5]);
}



translate([0,0,5+10*$t])
rotate([0,0,0])
%plug() ;

charger_plug();