module aligned_cube(size, aligned=[1,1,0])
{
    translate(-0.5*[size[0]*aligned[0], size[1]*aligned[1], size[2]*aligned[2]])
        cube(size);
}
module aligned_rounded_cube(size, r=5, rounding=[1,1,1], aligned=[1,1,0])
{

    translate(-0.5*[ ( size[0]*aligned[0] -2*r),
                     ( size[1]*aligned[1] -2*r),
                     ( size[2]*aligned[2] -2*r)])
    {

        _eps=min([size[0]/2,size[1]/2,size[2]/2, 0.001]);
        if( rounding == [1,1,1] ) {
            if(r>=min(size)) {
                echo("warning r is smaller than thickness for aligned cube");
            }
            minkowski() {
                cube([size[0] - 2*r, size[1] - 2*r, size[2]- 2*r]);
                sphere(r);
            }
        }else if( rounding == [1,1,0] ) {
            //cylinder rounding x/y
            if(r*2>=size[0] || r*2>=size[1] ) {
                echo("warning r is smaller than thickness for aligned cube");
            }
            minkowski() {
                translate([ 0,0,-r])
                    cube([size[0] - 2*r, size[1] - 2*r, size[2]-2*_eps]);
                cylinder(r=r,h=_eps);
            }
        }else if( rounding == [1,0,1] ) {
            //cylinder rounding x/z
            if(r*2>=size[0] || r*2>=size[2] ) {
                echo("warning r is smaller than thickness for aligned cube");
            }
            minkowski() {
                translate([ 0,-r,0])
                    cube([size[0] - 2*r, size[1] - 2*_eps, size[2]- 2*r]);
                rotate([90,0]) {
                    cylinder(r=r,h=_eps);
                }
            }
        }else if( rounding == [0,1,1] ) {
            //cylinder rounding x/z
            if(r*2>=size[1] || r*2>=size[2] ) {
                echo("warning r is smaller than thickness for aligned cube");
            }
            minkowski() {
                translate([-r,0,0])
                    cube([size[0] - 2*_eps , size[1] - 2*r, size[2]- 2*r]);
                rotate([0,90]) {
                    cylinder(r=r,h=_eps);
                }
            }
        }
    }
}


module rounded_cube(size, r, rounding=[0,0,0])
{
    aligned_rounded_cube(size, r, rounding=[0,0,0], aligned=[0,0,0]);
}



module axis(l=30, w=0.3) {
    color("red")
        cube([l,w,w]);
    color("lightblue")
        cube([w,w,l]);
    color("lightgreen")
        cube([w,l,w]);
    color("black")
        cube([w,w,w]);
}

module cylinder_square(size,d,aligned) {
    r=d/2;
    
    // front
    translate([-1*aligned[0]*(size[0]/2),-1*aligned[1]*(size[1]/2),-1*aligned[2]*d/4])
    translate([size[0]/2,size[1]/2,d/4])
    for (x=[1,-1]){
        hull() {
            translate([size[0]/2,x*size[1]/2,0])
            sphere(d=d);
            translate([-1*(size[0]/2),x*size[1]/2,0])
            sphere(d=d);
        }
        hull() {
            translate([x*size[0]/2,size[1]/2,0])
            sphere(d=d);
            translate([x*size[0]/2,-1*size[1]/2,0])
            sphere(d=d);
        }
    }
}
module cubic_flange($fn, size, e, aligned)
{
    h_off= (e < 0 ) ? 0 : size[2];

    difference()
    {
        aligned_cube(size=[size[0],size[1],size[2]], aligned=aligned);
        
        echo(e);
        echo(size[2]);
        translate([0,0,size[0]/2])
        scale([1,1,size[0]/e])
        cylinder_square($fn=$fn,size=[size[0],size[1]], d=e*2, aligned=aligned);
        //minkowski()
        
    }
}
// test show
$fn=32;
e=3;

translate([-45,45,0]) {
    
    translate([50,-90,0]) {
        axis();
        translate([0,0,10])
        %aligned_cube([10-2*e,10-2*e,5]);
        translate([0,0,-5])
        %aligned_cube([10,10,5]);

        cubic_flange($fn=32,size=[10,10,10], e=e, aligned=[1,1,0]); 
        translate([0,-10,0])
        text(halign="center",valign="center", size=2, "cubic_flange()");
    
    }

    translate([0,-90,0]) {
        axis();

        cube(size=[10,10,10]);
        translate([0,-10,0])
        text(halign="center",valign="center", size=2, "cube()");
    }

    translate([0,10,0]) {
        axis();

        aligned_cube(size=[10,10,10], aligned=[1,1,1]);
        translate([0,-10,0])
        text(halign="center",valign="center",  size=2, "aligned_cube([1,1,1])");
    }

    translate([0,-30,0]) {
        axis();

        aligned_rounded_cube(size=[10,10,10],r=1, aligned=[1,1,0]);
        translate([0,-10,0])
        text(halign="center",valign="center", size=2, "aligned_rounded_cube([1,1,0])");
    }

    translate([50,10,0]) {
        $fn=32;
        axis();

        aligned_rounded_cube(size=[10,10,10],r=1, aligned=[1,1,0]);
        translate([0,-10,0])
        text(halign="center",valign="center",  size=2, "aligned_rounded_cube(r=1)");
    }

    translate([50,-30,0]) {
        $fn=32;
        axis();

        aligned_rounded_cube(size=[10,10,10],r=2, aligned=[1,1,0]);
        translate([0,-10,0])
        text(halign="center",valign="center", size=2, "aligned_rounded_cube(r=2)");
    }

    translate([100,10,0]) {
        $fn=32;
        axis();

        aligned_rounded_cube(size=[10,10,10],r=2, rounding=[1,1,1]);
        translate([0,-10,0])
        text(halign="center",valign="center",  size=2, "aligned_rounded_cube([1,1,1])");
    }
    translate([100,-30,0]) {
        $fn=32;
        axis();

        aligned_rounded_cube(size=[10,10,10],r=2, rounding=[1,1,0]);
        translate([0,-10,0])
        text(halign="center",valign="center", size=2, "aligned_rounded_cube([1,1,0])");
    }
    translate([100,-60,0]) {
        $fn=32;
        axis();

        aligned_rounded_cube(size=[10,10,10],r=2, rounding=[1,0,1]);
        translate([0,-10,0])
        text(halign="center",valign="center",  size=2, "aligned_rounded_cube([1,0,1])");
    }
    translate([100,-90,0]) {
        $fn=32;
        axis();

        aligned_rounded_cube(size=[10,10,10],r=2, rounding=[0,1,1]);
        translate([0,-10,0])
        text(halign="center",valign="center", size=2, "aligned_rounded_cube([0,1,1])");
    }
}
