#!/usr/bin/perl
$src = $ARGV[0];
$dst = $ARGV[1];

$prod = $ENV{"PRODUCT_NAME"};

if ( $prod eq "wolf3d" ) {
    print "We don't scrub the non-lite app.\n";
    exit 0;
}

if ( -d "$dst/base/maps" ) {
   
   # copy appropriate splash image
   if ( -f "$src/../code/iphone/default_$prod.png" ) {
      print "Copying default_$prod.png as default splash image.\n";
      `cp $src/../code/iphone/default_$prod.png $dst/Default.png`;
   }
}
