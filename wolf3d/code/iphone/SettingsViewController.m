/*
 
 Copyright (C) 2011 id Software LLC, a ZeniMax Media company. 

 This file is part of the WOLF3D iOS v2.1 GPL Source Code. 

 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 
 */

#import "SettingsViewController.h"
#import "wolfiphone.h"
#import "wolf3dAppDelegate.h"

#define NUM_CONTROL_SCHEMES 4

void setSliderLabel( UISlider * slider, UILabel * label );
void setSliderValue( UISlider * slider, UILabel * label, float value );
float convertSliderValue( UISlider * slider );

@interface SettingsViewController ()

@property (nonatomic, retain) UISlider * sensitivitySlider;
@property (nonatomic, retain) UISlider * tiltMoveSpeedSlider;
@property (nonatomic, retain) UISlider * tiltTurnSpeedSlider;
@property (nonatomic, retain) UISlider * hudAlphaSlider;

@property (nonatomic, retain) UIView * sensitivityLabel;
@property (nonatomic, retain) UIView * tiltMoveSpeedLabel;
@property (nonatomic, retain) UIView * tiltTurnSpeedLabel;
@property (nonatomic, retain) UIView * hudAlphaLabel;

@property (nonatomic, retain) UILabel * presetLabel;


- (void)setPresetVisibility;

@end

@implementation SettingsViewController

@synthesize sensitivitySlider, tiltMoveSpeedSlider, tiltTurnSpeedSlider, hudAlphaSlider;
@synthesize sensitivityLabel, tiltMoveSpeedLabel, tiltTurnSpeedLabel, hudAlphaLabel;
@synthesize presetLabel;

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad]; 
    
    [self LoadSettings];
}

- (void) SetupSlider:(UISlider*)slider minimumTrack:(UIImage*)minImage
						maximumTrack:(UIImage*)maxImage
						thumb:(UIImage*)thumbImage {
	
	[slider setMinimumTrackImage:minImage forState:UIControlStateNormal];
	[slider setMaximumTrackImage:maxImage forState:UIControlStateNormal];
			   		   
	[slider setThumbImage:thumbImage forState:UIControlStateNormal];
	[slider setThumbImage:thumbImage forState:UIControlStateHighlighted];
}

- (void) LoadSettings {
	// Minimum track image setup.
	UIImage* minimumTrackImage = [UIImage imageNamed:@"settings_slider_grey.png"];
	CGFloat minimumTrackImageCap = minimumTrackImage.size.width * 0.5f;
	
	UIImage* minimumTrackImageCapped = [minimumTrackImage stretchableImageWithLeftCapWidth:minimumTrackImageCap topCapHeight:0.0];


	// Maximum track image setup.
	UIImage* maximumTrackImage = [UIImage imageNamed:@"settings_slider_white.png"];
	CGFloat maximumTrackImageCap = maximumTrackImage.size.width * 0.5f;
	
	UIImage* maximumTrackImageCapped = [maximumTrackImage stretchableImageWithLeftCapWidth:maximumTrackImageCap topCapHeight:0.0];

	// Thumb image.
	UIImage* thumbImage = [UIImage imageNamed:@"settings_slider_blue.png"];

	// Set up slider instances.
	[self SetupSlider:sensitivitySlider minimumTrack:minimumTrackImageCapped
										maximumTrack:maximumTrackImageCapped
										thumb:thumbImage];

   
	[self SetupSlider:tiltMoveSpeedSlider minimumTrack:minimumTrackImageCapped
										  maximumTrack:maximumTrackImageCapped
										  thumb:thumbImage];

   
	
	[self SetupSlider:tiltTurnSpeedSlider minimumTrack:minimumTrackImageCapped
										  maximumTrack:maximumTrackImageCapped
										  thumb:thumbImage];




	[self SetupSlider:hudAlphaSlider minimumTrack:minimumTrackImageCapped
									 maximumTrack:maximumTrackImageCapped
									 thumb:thumbImage];



    // Load Settings from CVars.
    setSliderValue( sensitivitySlider, sensitivityLabel, sensitivity->value );
    setSliderValue( tiltMoveSpeedSlider, tiltMoveSpeedLabel, tiltMove->value );
    setSliderValue( tiltTurnSpeedSlider, tiltTurnSpeedLabel, tiltTurn->value );
    setSliderValue( hudAlphaSlider, hudAlphaLabel, hudAlpha->value );

	[self setPresetVisibility];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
	
	self.sensitivitySlider = nil;
	self.tiltTurnSpeedSlider = nil;
	self.tiltMoveSpeedSlider = nil;
	self.hudAlphaSlider = nil;
	
	self.sensitivityLabel = nil;
	self.tiltTurnSpeedLabel = nil;
	self.tiltMoveSpeedLabel = nil;
	self.hudAlphaSlider = nil;
	
	self.presetLabel = nil;
}


- (void)dealloc {
    [super dealloc];
}

 
- (IBAction)back:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}

- (IBAction) SensitivityValChanged {
   Cvar_SetValue( sensitivity->name, convertSliderValue( sensitivitySlider ) );
   setSliderLabel( sensitivitySlider, sensitivityLabel);
}


- (IBAction) TiltMoveSpeedValChanged {
    Cvar_SetValue( tiltMove->name, convertSliderValue( tiltMoveSpeedSlider ) );
    setSliderLabel( tiltMoveSpeedSlider, tiltMoveSpeedLabel);
	
    if ( tiltMove->value == 5000 ) {
		Cvar_SetValue( tiltMove->name, 0 );
	}
	if ( tiltMove->value ) {
		Cvar_SetValue( tiltTurn->name, 0 );
        setSliderValue( tiltTurnSpeedSlider, tiltTurnSpeedLabel, tiltTurnSpeedSlider.minimumValue );
	}
}

- (IBAction) TiltTurnSpeedValChanged {
    Cvar_SetValue( tiltTurn->name, convertSliderValue( tiltTurnSpeedSlider ) );
    setSliderLabel( tiltTurnSpeedSlider, tiltTurnSpeedLabel);
    
    if ( tiltTurn->value == 500 ) {
		Cvar_SetValue( tiltTurn->name, 0 );
	}
	if ( tiltTurn->value ) {
		Cvar_SetValue( tiltMove->name, 0 );
        setSliderValue( tiltMoveSpeedSlider, tiltMoveSpeedLabel, tiltMoveSpeedSlider.minimumValue );
	}
}

- (IBAction) HudAlphaValChanged {
    Cvar_SetValue( hudAlpha->name, convertSliderValue( hudAlphaSlider ) );
	setSliderLabel( hudAlphaSlider, hudAlphaLabel );
}

- (IBAction) AdvancedPressed {
    
    iphonePreloadBeforePlay();	// make sure all the hud textures are loaded	
    menuState = IPM_HUDEDIT;
	wolf3dAppDelegate* app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	[app showOpenGL];
    
}

- (void)setPresetVisibility {
	
	for ( int i = 0; i < NUM_CONTROL_SCHEMES; ++i ) {
		BOOL hide = true;
		
		if ( i == (int)controlScheme->value ) {
			hide = false;
		}
		
		[[self.view viewWithTag:(i + 1)] setHidden:hide];
	}
	
	self.presetLabel.text = [NSString stringWithFormat:@"Preset %d", (int)(controlScheme->value + 1) ];
}

- (IBAction)nextPreset {
	int newScheme = controlScheme->value + 1;
	
	if ( newScheme >= NUM_CONTROL_SCHEMES ) {
		newScheme = 0;
	}
	
	Cvar_SetValue( controlScheme->name, newScheme );
	HudSetForScheme( (int)controlScheme->value );
	
	[self setPresetVisibility];
}

- (IBAction)previousPreset {
	int newScheme = controlScheme->value - 1;
	
	if ( newScheme < 0 ) {
		newScheme = NUM_CONTROL_SCHEMES - 1;
	}
	
	Cvar_SetValue( controlScheme->name, newScheme );
	HudSetForScheme( (int)controlScheme->value );
	
	[self setPresetVisibility];
}

@end

float convertSliderValue( UISlider * slider ) {
	const float difference = slider.value - slider.minimumValue;
	const float adjustedValue = slider.maximumValue - difference;

	return adjustedValue;
}

void setSliderValue( UISlider * slider, UILabel * label, float value ) {
	const float difference = value - slider.minimumValue;
	const float reversedValue = slider.maximumValue - difference;
	
	slider.value = reversedValue;
	
	setSliderLabel( slider, label );
}

void setSliderLabel( UISlider * slider, UILabel * label ) {
	const float range = slider.maximumValue - slider.minimumValue;
	const float adjustedValue = convertSliderValue( slider );
	
	const float adjustedDiff = adjustedValue - slider.minimumValue;
	
	const float percent = adjustedDiff / range * 100.0f;
	
	label.text = [NSString stringWithFormat:@"%.0f%%", percent];
}

