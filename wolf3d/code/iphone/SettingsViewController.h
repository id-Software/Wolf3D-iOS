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

#import <UIKit/UIKit.h>


@interface SettingsViewController : UIViewController {

#if !TARGET_OS_TV
    IBOutlet UISlider * sensitivitySlider;
    IBOutlet UISlider * tiltMoveSpeedSlider;
    IBOutlet UISlider * tiltTurnSpeedSlider;
    IBOutlet UISlider * hudAlphaSlider;
#endif
    
    IBOutlet UILabel * sensitivityLabel;
	IBOutlet UILabel * tiltMoveSpeedLabel;
	IBOutlet UILabel * tiltTurnSpeedLabel;
	IBOutlet UILabel * hudAlphaLabel;
	
	IBOutlet UILabel * presetLabel;
}

- (void) LoadSettings;

- (IBAction)back:(id)sender;

- (IBAction) AdvancedPressed;

- (IBAction) SensitivityValChanged;
- (IBAction) TiltMoveSpeedValChanged;
- (IBAction) TiltTurnSpeedValChanged;
- (IBAction) HudAlphaValChanged;

- (IBAction) nextPreset;
- (IBAction) previousPreset;

@end
