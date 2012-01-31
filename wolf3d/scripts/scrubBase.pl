#!/usr/bin/perl
$src = $ARGV[0];
$dst = $ARGV[1];

$prod = $ENV{"PRODUCT_NAME"};

if ( $prod eq "wolf3d" ) {
    print "We don't scrub the non-lite app.\n";
    exit 0;
}

if ( -d "$dst/base/maps" ) {
   print "Scrubbing maps for $prod.\n";
   chdir "$dst/base/maps";
   opendir( DIR, "." );
   my @files = readdir( DIR );
   closedir( DIR );
   #print "Candidates are: " . join( " ", @files ) . "\n";
   @files = grep { /^w[1-9]\d\.map$/ || /^w0[3-9]\.map$/ } @files;
   #print "Filtered are: " . join( " ", @files ) . "\n";
   unlink @files;
   
   # copy appropriate splash image
   if ( -f "$src/../code/iphone/default_$prod.png" ) {
      print "Copying default_$prod.png as default splash image.\n";
      `cp $src/../code/iphone/default_$prod.png $dst/Default.png`;
   }

   # copy appropriate victory image
   if ( -f "$src/../code/iphone/victory_$prod.tga" ) {
      print "Copying victory_$prod.tga as victory image.\n";
      `rm $dst/base/iphone/victory_256.5551`;
      `cp $src/../code/iphone/victory_$prod.tga $dst/base/iphone/victory_256.tga`;
   }
}
