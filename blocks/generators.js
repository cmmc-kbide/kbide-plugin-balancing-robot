// Blockly.JavaScript['kb_balancing_plugin.random-text'] = function (block) {
//   return [`DEV_I2C0.kb_balancing_plugin().random()`, Blockly.JavaScript.ORDER_ATOMIC]
// }

// Blockly.JavaScript['kb_balancing_plugin.basic_string'] = function (block) {
//   return [`DEV_I2C0.kb_balancing_plugin().random()`, Blockly.JavaScript.ORDER_ATOMIC]
// }

Blockly.JavaScript['kb_bb_reset'] = function (block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'DEV_I2C0.kb_balancing_plugin().set_reboot();\n';
  return code;
};

Blockly.JavaScript['kb_bb_set_sensor_offset'] = function (block) {
  var value_var1 = Blockly.JavaScript.valueToCode(block, 'var1', Blockly.JavaScript.ORDER_ATOMIC);
  var value_var2 = Blockly.JavaScript.valueToCode(block, 'var2', Blockly.JavaScript.ORDER_ATOMIC);
  var value_var3 = Blockly.JavaScript.valueToCode(block, 'var3', Blockly.JavaScript.ORDER_ATOMIC);
  var value_var4 = Blockly.JavaScript.valueToCode(block, 'var4', Blockly.JavaScript.ORDER_ATOMIC);
  var value_var5 = Blockly.JavaScript.valueToCode(block, 'var5', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'DEV_I2C0.kb_balancing_plugin().set_sensoroffset(' + value_var1 + ',' + value_var2 + ',' + value_var3 + ',' + value_var4 + ',' + value_var5 + ');\n';
  return code;
};

Blockly.JavaScript['kb_bb_stand'] = function (block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'DEV_I2C0.kb_balancing_plugin().get_RobotStand()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['kb_bb_read_sensor'] = function (block) {
  var dropdown_var1 = block.getFieldValue('var1');
  var dropdown_var2 = block.getFieldValue('var2');
  // TODO: Assemble JavaScript into code variable.
  var code = 'DEV_I2C0.kb_balancing_plugin().read_sensor(' + dropdown_var1 + ',' + dropdown_var2 + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['kb_bb_read_motor'] = function (block) {
  var dropdown_var1 = block.getFieldValue('var1');
  var dropdown_var2 = block.getFieldValue('var2');
  // TODO: Assemble JavaScript into code variable.
  var code = 'DEV_I2C0.kb_balancing_plugin().read_wheel(' + dropdown_var1 + ',' + dropdown_var2 + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['kb_bb_set_command'] = function (block) {
  var dropdown_var1 = block.getFieldValue('var1');
  var value_var1 = Blockly.JavaScript.valueToCode(block, 'var1', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'DEV_I2C0.kb_balancing_plugin().write_command(' + dropdown_var1 + ',' + value_var1 + ');\n';
  return code;
};

Blockly.JavaScript['kb_bb_set_gain'] = function (block) {
  var dropdown_var1 = block.getFieldValue('var1');
  var value_var1 = Blockly.JavaScript.valueToCode(block, 'var1', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'DEV_I2C0.kb_balancing_plugin().set_pid_gain(' + dropdown_var1 + ',' + value_var1 + ');\n';
  return code;
};

Blockly.JavaScript['kb_bb_set_maximum'] = function (block) {
  var dropdown_var1 = block.getFieldValue('var1');
  var value_var1 = Blockly.JavaScript.valueToCode(block, 'var1', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'DEV_I2C0.kb_balancing_plugin().set_limit(' + dropdown_var1 + ',' + value_var1 + ');\n';
  return code;
};

Blockly.JavaScript['kb_bb_calsensor'] = function (block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'DEV_I2C0.kb_balancing_plugin().cal_sensor();\n';
  return code;
};

Blockly.JavaScript['kb_bb_mode'] = function (block) {
  var checkbox_v1 = block.getFieldValue('v1') == 'TRUE';
  var checkbox_v2 = block.getFieldValue('v2') == 'TRUE';
  var checkbox_v3 = block.getFieldValue('v3') == 'TRUE';
  var checkbox_v4 = block.getFieldValue('v4') == 'TRUE';
  var checkbox_v5 = block.getFieldValue('v5') == 'TRUE';
  var checkbox_v6 = block.getFieldValue('v6') == 'TRUE';
  var checkbox_v7 = block.getFieldValue('v7') == 'TRUE';
  // TODO: Assemble JavaScript into code variable.
  var code = 'DEV_I2C0.kb_balancing_plugin().set_mode(' + checkbox_v1 + ',' + checkbox_v2 + ',' + checkbox_v3 + ',' + checkbox_v4 + ',' + checkbox_v5 + ',' + checkbox_v6 + ',' + checkbox_v7 +');\n';
  return code;
};

Blockly.JavaScript['kb_bb_position'] = function(block) {
  var dropdown_name = block.getFieldValue('NAME');
  var value_v1 = Blockly.JavaScript.valueToCode(block, 'v1', Blockly.JavaScript.ORDER_ATOMIC);
  var value_v2 = Blockly.JavaScript.valueToCode(block, 'v2', Blockly.JavaScript.ORDER_ATOMIC);
  var value_v3 = Blockly.JavaScript.valueToCode(block, 'v3', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'DEV_I2C0.kb_balancing_plugin().position_control_mode(' + dropdown_name + ',' + value_v1 + ',' + value_v2 + ',' + value_v3 + ');\n';
  return code;
};



























