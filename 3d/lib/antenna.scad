$fn=32;
eps=0.01;
module antenna() {

    d1=[12.8, 12.8, 12.8, 5.85, 5.85, 5.3, 5.85,  5.85,];
    d2=[12.8, 12.8, 12.8, 12.8, 5.85, 5.3, 5.85,  5.85,];
    h= [180.8, 172,  154,  81,   44,   12, 10.60, 0,   ];

    difference()
    {
        translate([0, 0, -h[0]]) {
            sphere(d=d1[len(d1)-1]);
            for (i=[0:len(h)-1]) {
                translate([0, 0, h[i+1]])
                    cylinder(d1=d1[i], d2=d2[i], h=h[i]-h[i+1]);
            }
        }
        translate([0, 0, -5.1]) {
            sma_connector_male();
        }
    }
}

module sma_connector_male() {
    d=[2,    3.2,   9.1,    8,  9.1,    6.15, ];
    fn=[0,    0,     6,    0,     6,    0   , ];
    h= [20.5, 16.45, 13.1, 10.75, 8.75, 7.1 , 0];

    //translate([0, 0, -h[0]])
    {
        difference() {
            for (i=[0:len(h)-1]) {
                translate([0, 0, h[i+1]]) {
                    if(fn[i] == 0)
                        cylinder(d=d[i], h=h[i]-h[i+1]);
                    else
                        cylinder($fn=fn[i], d=d[i], h=h[i]-h[i+1]);
                }
            }

            translate([0, 0, -eps]) {
                cylinder(d=4.6,h=1.75);
            }
        }
        cylinder(d=0.92,h=1.75);
    }
}

translate([0,0,0 ])
    sma_connector_male();

translate([0,0,-10])
    antenna();
