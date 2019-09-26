module.exports = [
    {
        name: "Balancing Robot",
        blocks: [
            {
                xml: `<sep gap="32"></sep><label text="CMMC Balancing Robot" web-class="headline"></label>`
            },
            {
                xml: `<sep gap="32"></sep><label text="P.I.D. Setting" web-class="headline"></label>`
            },
            'kb_bb_reset',
            'kb_bb_stand',
            {
                xml:
                    `<block type="kb_bb_set_sensor_offset">
                        <value name="var1">
                            <block type = "math_number">
                                <field name="NUM">0</field>
                            </block>
                        </value>
                        <value name="var2">
                            <block type = "math_number">
                                <field name="NUM">0</field>
                            </block>
                        </value>
                        <value name="var3">
                            <block type = "math_number">
                                <field name="NUM">0</field>
                            </block>
                        </value>
                        <value name="var4">
                            <block type = "math_number">
                                <field name="NUM">0</field>
                            </block>
                        </value>
                        <value name="var5">
                            <block type = "math_number">
                                <field name="NUM">0</field>
                            </block>
                        </value>
                    </block>`
            },
            'kb_bb_read_sensor',
            'kb_bb_read_motor',
            {
                xml: `<sep gap="32"></sep><label text="Operating Mode" web-class="headline"></label>`
            },
            'kb_bb_set_gain',
            'kb_bb_set_maximum',
            'kb_bb_mode',
            {
                xml: `<sep gap="32"></sep><label text="Calibration Sensor" web-class="headline"></label>`
            },
            'kb_bb_calsensor',
            {
                xml: `<sep gap="32"></sep><label text="Send Command" web-class="headline"></label>`
            },
            'kb_bb_set_command'
        ]
    }
];