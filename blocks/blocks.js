// Blockly.Blocks["kb_balancing_plugin.random-text"] = {
//     init: function () {
//         this.appendDummyInput()
//             .appendField(Blockly.Msg.kb_balancing_plugin_TEXT_TITLE);

//         // this.appendDummyInput()
//         // 	.appendField(Blockly.Msg.CHANNEL)
//         // 	.appendField(new Blockly.FieldDropdown(channel_array), 'CHANNEL');
//         // device addess
//         // this.appendDummyInput()
//         // 	.appendField(Blockly.Msg.ADDRESS)
//         // 	.appendField(new Blockly.FieldDropdown([
//         // 		["0x29", "41"],
//         // 		["0x39", "57"],
//         // 		["0x49", "73"]
//         // 	]), 'ADDRESS');

//         this.setOutput(true, 'String');
//         this.setInputsInline(true)
//         this.setPreviousStatement(false);
//         this.setNextStatement(false);
//         this.setColour(58);
//         this.setTooltip(Blockly.Msg.kb_balancing_plugin_TEXT_TOOLTIP);
//         this.setHelpUrl(Blockly.Msg.kb_balancing_plugin_TEXT_HELPURL);
//     }
// };

// Blockly.Blocks["kb_balancing_plugin.basic_string"] = {
//   init: function() {
//     this.appendDummyInput()
//       .appendField(new Blockly.FieldImage('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAKCAQAAAAqJXdxAAAAn0lEQVQI1z3OMa5BURSF4f/cQhAKjUQhuQmFNwGJEUi0RKN5rU7FHKhpjEH3TEMtkdBSCY1EIv8r7nFX9e29V7EBAOvu7RPjwmWGH/VuF8CyN9/OAdvqIXYLvtRaNjx9mMTDyo+NjAN1HNcl9ZQ5oQMM3dgDUqDo1l8DzvwmtZN7mnD+PkmLa+4mhrxVA9fRowBWmVBhFy5gYEjKMfz9AylsaRRgGzvZAAAAAElFTkSuQmCC', 12, 12, '*'))
//       .appendField(new Blockly.FieldTextInput('Hello World!'), 'VALUE')
//       .appendField(new Blockly.FieldImage('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAKCAQAAAAqJXdxAAAAqUlEQVQI1z3KvUpCcRiA8ef9E4JNHhI0aFEacm1o0BsI0Slx8wa8gLauoDnoBhq7DcfWhggONDmJJgqCPA7neJ7p934EOOKOnM8Q7PDElo/4x4lFb2DmuUjcUzS3URnGib9qaPNbuXvBO3sGPHJDRG6fGVdMSeWDP2q99FQdFrz26Gu5Tq7dFMzUvbXy8KXeAj57cOklgA+u1B5AoslLtGIHQMaCVnwDnADZIFIrXsoXrgAAAABJRU5ErkJggg==', 12, 12, '*'));
//     this.setOutput(true, 'String');
//     this.setPreviousStatement(false);
//     this.setNextStatement(false);
//     this.setColour(basic_colour);
//     this.setTooltip(Blockly.Msg.BASIC_STRING_TOOLTIP);
//     this.setHelpUrl(Blockly.Msg.BASIC_STRING_HELPURL);
//   }
// };

Blockly.Blocks['kb_bb_reset'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Reset Robot");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(345);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['kb_bb_set_sensor_offset'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Set Sensor Offset:");
    this.appendValueInput("var1")
        .setCheck(null)
        .appendField("GyroX=");
    this.appendValueInput("var2")
        .setCheck(null)
        .appendField("GyroY=");
    this.appendValueInput("var3")
        .setCheck(null)
        .appendField("GyroZ=");
    this.appendValueInput("var4")
        .setCheck(null)
        .appendField("AccX=");
    this.appendValueInput("var5")
        .setCheck(null)
        .appendField("AccY=");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(345);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['kb_bb_stand'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Are Robot Stand?");
    this.setInputsInline(true);
    this.setOutput(true, null);
    this.setColour(345);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['kb_bb_read_sensor'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Read Robot:")
        .appendField(new Blockly.FieldDropdown([["Angle","0"], ["Angular Velocity","1"], ["Acceleration","2"]]), "var1")
        .appendField("Axis:")
        .appendField(new Blockly.FieldDropdown([["X","0"], ["Y","1"], ["Z","2"]]), "var2");
    this.setInputsInline(true);
    this.setOutput(true, null);
    this.setColour(345);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['kb_bb_read_motor'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Read Wheel:")
        .appendField(new Blockly.FieldDropdown([["Positoin","0"], ["Speed","1"], ["Power","2"]]), "var1")
        .appendField("Axis:")
        .appendField(new Blockly.FieldDropdown([["L","0"], ["R","1"]]), "var2");
    this.setInputsInline(true);
    this.setOutput(true, null);
    this.setColour(345);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['kb_bb_set_command'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Write Command Channel:");
    this.appendValueInput("var1")
        .setCheck(null)
        .appendField(new Blockly.FieldDropdown([["A","0"], ["B","1"]]), "var1")
        .appendField("Value=");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(345);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['kb_bb_set_gain'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Set PID Gain:");
    this.appendValueInput("var1")
        .setCheck(null)
        .appendField(new Blockly.FieldDropdown([["Kp Pitch Control","0"], ["Kd Pitch Control","1"], ["Kp Yaw Control","2"], ["Ki Yaw Control","3"], ["Kp Speed Control","4"], ["Ki Speed Control","5"]]), "var1")
        .appendField("Value=");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(345);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['kb_bb_set_maximum'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Set Limit:");
    this.appendValueInput("var1")
        .setCheck(null)
        .appendField(new Blockly.FieldDropdown([["Maximum Pitch Angle","0"], ["Maximum Robot Speed","1"]]), "var1")
        .appendField("Value=");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(345);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['kb_bb_calsensor'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Robot Calibration Sensor ");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(345);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['kb_bb_mode'] = {
  init: function() {
    this.appendDummyInput()
        .setAlign(Blockly.ALIGN_CENTRE)
        .appendField("Robot Operating Mode");
    this.appendDummyInput()
        .appendField(new Blockly.FieldCheckbox("FALSE"), "v1")
        .appendField("Manual Motor Control");
    this.appendDummyInput()
        .appendField(new Blockly.FieldCheckbox("FALSE"), "v2")
        .appendField("Motor Torque Control ");
    this.appendDummyInput()
        .appendField(new Blockly.FieldCheckbox("TRUE"), "v3")
        .appendField("PD Robot Pitch Angle Controller");
    this.appendDummyInput()
        .appendField(new Blockly.FieldCheckbox("TRUE"), "v4")
        .appendField("PI Robot Speed Controller");
    this.appendDummyInput()
        .appendField(new Blockly.FieldCheckbox("TRUE"), "v5")
        .appendField("PI Robot Steering Controller");
    this.appendDummyInput()
        .appendField(new Blockly.FieldCheckbox("FALSE"), "v6")
        .appendField("Robot Turning \"S\" mode");
    this.appendDummyInput()
        .appendField(new Blockly.FieldCheckbox("FALSE"), "v7")
        .appendField("Robot Position Control mode");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(345);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};


// Blockly.Blocks['kb_bb_position'] = {
//     init: function() {
//       this.appendDummyInput()
//           .appendField("Robot Move")
//           .appendField(new Blockly.FieldDropdown([["Forward","0"], ["Backward","1"], ["Turn Left","2"], ["Turn Right","3"]]), "NAME");
//       this.appendValueInput("v1")
//           .setCheck(null)
//           .setAlign(Blockly.ALIGN_RIGHT)
//           .appendField("Distance (cm,degree)");
//       this.appendValueInput("v2")
//           .setCheck(null)
//           .setAlign(Blockly.ALIGN_RIGHT)
//           .appendField("Speed (cm/s,degree/s)");
//       this.appendValueInput("v3")
//           .setCheck(null)
//           .setAlign(Blockly.ALIGN_RIGHT)
//           .appendField("Finish gap (cm,degree)");
//       this.setPreviousStatement(true, null);
//       this.setNextStatement(true, null);
//       this.setColour(345);
//    this.setTooltip("");
//    this.setHelpUrl("");
//     }
//   };
  































