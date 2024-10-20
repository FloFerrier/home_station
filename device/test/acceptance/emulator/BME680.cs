//
// Copyright (c) 2010-2023 Antmicro
//
// This file is licensed under the MIT License.
// Full license text is available in 'licenses/MIT.txt'.
//

using System;
using System.Linq;
using System.Collections.Generic;
using Antmicro.Renode.Peripherals.Bus;
using Antmicro.Renode.Logging;
using Antmicro.Renode.Core;
using Antmicro.Renode.Core.Structure.Registers;
using Antmicro.Renode.Utilities;

namespace Antmicro.Renode.Peripherals.I2C {
    public class BME680 : II2CPeripheral, IProvidesRegisterCollection<ByteRegisterCollection> {
        public BME680() {
            RegistersCollection = new ByteRegisterCollection(this);
            gasWait = new IValueRegisterField[10];
            resHeat = new IValueRegisterField[10];
            idacHeat = new IValueRegisterField[10];
            gas_r = new IValueRegisterField[2];
            hum = new IValueRegisterField[2];
            temp = new IValueRegisterField[3];
            press = new IValueRegisterField[3];
            DefineRegisters();
            Reset();
        }

        public void Reset() {
            RegistersCollection.Reset();
            selectedRegister = 0x0;
            state = State.Idle;
        }

        public void Write(byte[] data) {
            this.Log(LogLevel.Warning, "Write {0}", data.Select(x => x.ToString("X")).Aggregate((x, y) => x + " " + y));
            foreach(var b in data) {
                switch(state) {
                    case State.Idle:
                    case State.Reading:
                        selectedRegister = (Registers)b;
                        state = State.ReceivedFirstByte;
                        break;
                    case State.ReceivedFirstByte:
                    case State.WritingWaitingForValue:
                        RegistersCollection.Write((byte)selectedRegister, b); //BME680 have 256 addressable registers the same as byte max value
                        state = State.WaitingForAddress;
                        break;
                    case State.WaitingForAddress:
                        selectedRegister = (Registers)b;
                        state = State.WritingWaitingForValue;
                        break;
                }
            }
        }

        public byte[] Read(int count = 0) {
            byte[] buf = new byte[count];
            state = State.Reading; //reading can be started regardless of state, last selectedRegister is used
            for(int i = 0; i < buf.Length; i++) {
                //BME680 have 256 addressable registers, byte covers them all and allows roll-over like in real hardware
                buf[i] = RegistersCollection.Read((byte)selectedRegister);
                this.Log(LogLevel.Warning, "Read count:{0} @0x{1} = {2}", count, selectedRegister.ToString("X"), buf.Select(x => x.ToString("X")).Aggregate((x, y) => x + " " + y));
                selectedRegister++;
            }

            return buf;
        }

        public void FinishTransmission() {
            this.Log(LogLevel.Warning, "Finish transmission");
            if(state != State.ReceivedFirstByte) //in case of reading we may (documentation permits this or repeated START) receive STOP before the read transfer
            {
                if(state == State.WritingWaitingForValue)
                {
                    this.Log(LogLevel.Warning, "Trying to write odd amount of bytes, last register is missing its value");
                }
                state = State.Idle;
            }
        }

        public ByteRegisterCollection RegistersCollection {
            get;
        }

        private void DefineRegisters() {
            Registers.Reset.Define(this, DEFAULT_DATA).WithValueField(0, 8)
                .WithWriteCallback((_, val) =>
                {
                    if(val == RESET_DATA)
                    {
                        Reset();
                    }
                });
            Registers.Id.Define(this, DEVICE_ID).WithValueField(0, 8, FieldMode.Read, valueProviderCallback: _ => DEVICE_ID);
            Registers.Status.Define(this, DEFAULT_DATA).WithFlag(4, out spi_mem_page, name: "SPI_MEM_PAGE");
            Registers.GasWait0.Define(this, DEFAULT_DATA).WithValueField(0, 8, out gasWait[0], name: "GAS_WAIT_0");
            Registers.GasWait1.Define(this, DEFAULT_DATA).WithValueField(0, 8, out gasWait[1], name: "GAS_WAIT_1");
            Registers.GasWait2.Define(this, DEFAULT_DATA).WithValueField(0, 8, out gasWait[2], name: "GAS_WAIT_2");
            Registers.GasWait3.Define(this, DEFAULT_DATA).WithValueField(0, 8, out gasWait[3], name: "GAS_WAIT_3");
            Registers.GasWait4.Define(this, DEFAULT_DATA).WithValueField(0, 8, out gasWait[4], name: "GAS_WAIT_4");
            Registers.GasWait5.Define(this, DEFAULT_DATA).WithValueField(0, 8, out gasWait[5], name: "GAS_WAIT_5");
            Registers.GasWait6.Define(this, DEFAULT_DATA).WithValueField(0, 8, out gasWait[6], name: "GAS_WAIT_6");
            Registers.GasWait7.Define(this, DEFAULT_DATA).WithValueField(0, 8, out gasWait[7], name: "GAS_WAIT_7");
            Registers.GasWait8.Define(this, DEFAULT_DATA).WithValueField(0, 8, out gasWait[8], name: "GAS_WAIT_8");
            Registers.GasWait9.Define(this, DEFAULT_DATA).WithValueField(0, 8, out gasWait[9], name: "GAS_WAIT_9");
            Registers.ResHeat0.Define(this, DEFAULT_DATA).WithValueField(0, 8, out resHeat[0], name: "RES_HEAT_0");
            Registers.ResHeat1.Define(this, DEFAULT_DATA).WithValueField(0, 8, out resHeat[1], name: "RES_HEAT_1");
            Registers.ResHeat2.Define(this, DEFAULT_DATA).WithValueField(0, 8, out resHeat[2], name: "RES_HEAT_2");
            Registers.ResHeat3.Define(this, DEFAULT_DATA).WithValueField(0, 8, out resHeat[3], name: "RES_HEAT_3");
            Registers.ResHeat4.Define(this, DEFAULT_DATA).WithValueField(0, 8, out resHeat[4], name: "RES_HEAT_4");
            Registers.ResHeat5.Define(this, DEFAULT_DATA).WithValueField(0, 8, out resHeat[5], name: "RES_HEAT_5");
            Registers.ResHeat6.Define(this, DEFAULT_DATA).WithValueField(0, 8, out resHeat[6], name: "RES_HEAT_6");
            Registers.ResHeat7.Define(this, DEFAULT_DATA).WithValueField(0, 8, out resHeat[7], name: "RES_HEAT_7");
            Registers.ResHeat8.Define(this, DEFAULT_DATA).WithValueField(0, 8, out resHeat[8], name: "RES_HEAT_8");
            Registers.ResHeat9.Define(this, DEFAULT_DATA).WithValueField(0, 8, out resHeat[9], name: "RES_HEAT_9");
            Registers.IdacHeat0.Define(this, DEFAULT_DATA).WithValueField(0, 8, out idacHeat[0], name: "IDAC_HEAT_0");
            Registers.IdacHeat1.Define(this, DEFAULT_DATA).WithValueField(0, 8, out idacHeat[1], name: "IDAC_HEAT_1");
            Registers.IdacHeat2.Define(this, DEFAULT_DATA).WithValueField(0, 8, out idacHeat[2], name: "IDAC_HEAT_2");
            Registers.IdacHeat3.Define(this, DEFAULT_DATA).WithValueField(0, 8, out idacHeat[3], name: "IDAC_HEAT_3");
            Registers.IdacHeat4.Define(this, DEFAULT_DATA).WithValueField(0, 8, out idacHeat[4], name: "IDAC_HEAT_4");
            Registers.IdacHeat5.Define(this, DEFAULT_DATA).WithValueField(0, 8, out idacHeat[5], name: "IDAC_HEAT_5");
            Registers.IdacHeat6.Define(this, DEFAULT_DATA).WithValueField(0, 8, out idacHeat[6], name: "IDAC_HEAT_6");
            Registers.IdacHeat7.Define(this, DEFAULT_DATA).WithValueField(0, 8, out idacHeat[7], name: "IDAC_HEAT_7");
            Registers.IdacHeat8.Define(this, DEFAULT_DATA).WithValueField(0, 8, out idacHeat[8], name: "IDAC_HEAT_8");
            Registers.IdacHeat9.Define(this, DEFAULT_DATA).WithValueField(0, 8, out idacHeat[9], name: "IDAC_HEAT_9");
            Registers.Config.Define(this, DEFAULT_DATA).WithFlag(0, out spi_3w_en, name: "SPI_3w_EN").WithValueField(2, 3, out filter, name: "FILTER");
            Registers.CtrlMeas.Define(this, DEFAULT_DATA).WithFlag(0, out mode, name: "MODE").WithValueField(2, 3, out osrs_p, name: "OSRS_P").WithValueField(5, 3, out osrs_t, name: "OSRS_T");
            Registers.CtrlHum.Define(this, DEFAULT_DATA).WithValueField(0, 3, out osrs_h, name: "OSRS_H").WithFlag(6, out spi_3w_int_en, name: "SPI_3W_INT_EN");
            Registers.CtrlGas0.Define(this, DEFAULT_DATA).WithFlag(3, out heat_off, name: "HEAT_OFF");
            Registers.CtrlGas1.Define(this, DEFAULT_DATA).WithValueField(0, 4, out nb_conv, name: "NB_CONV").WithFlag(4, out run_gas, name: "RUN_GAS");
            Registers.GasResLsb.Define(this, DEFAULT_DATA).WithValueField(0, 4, out gas_range_r, FieldMode.Read, name: "GAS_RANGE_R").WithFlag(4, out heat_stab_r, FieldMode.Read, name: "HEAT_STAB_R").WithFlag(5, out gas_valid_r, FieldMode.Read, name: "GAS_VALID_R").WithValueField(6, 2, out gas_r[0], FieldMode.Read, name: "GAS_R_LSB");
            Registers.GasResMsb.Define(this, DEFAULT_DATA).WithValueField(0, 8, out gas_r[1], FieldMode.Read, name: "GAS_R_MSB");
            Registers.HumLsb.Define(this, DEFAULT_DATA).WithValueField(0, 8, out hum[0], FieldMode.Read, name: "HUM_LSB");
            Registers.HumMsb.Define(this, 0x80).WithValueField(0, 8, out hum[1], FieldMode.Read, name: "HUM_MSB");
            Registers.TempXlsb.Define(this, DEFAULT_DATA).WithValueField(4, 4, out temp[0], FieldMode.Read, name: "TEMP_XLSB");
            Registers.TempLsb.Define(this, DEFAULT_DATA).WithValueField(0, 8, out temp[1], FieldMode.Read, name: "TEMP_LSB");
            Registers.TempMsb.Define(this, 0x80).WithValueField(0, 8, out temp[2], FieldMode.Read, name: "TEMP_MSB");
            Registers.PressXlsb.Define(this, DEFAULT_DATA).WithValueField(4, 4, out press[0], FieldMode.Read, name: "PRESS_XLSB");
            Registers.PressLsb.Define(this, DEFAULT_DATA).WithValueField(0, 8, out press[1], FieldMode.Read, name: "PRESS_LSB");
            Registers.PressMsb.Define(this, 0x80).WithValueField(0, 8, out press[2], FieldMode.Read, name: "PRESS_MSB");
            Registers.EasStatus0.Define(this, DEFAULT_DATA).WithValueField(0, 4, out gas_meas_index, FieldMode.Read, name: "GAS_MEAS_INDEX").WithFlag(5, out measuring, FieldMode.Read, name: "MEASURING").WithFlag(6, out gas_measuring, FieldMode.Read, name: "GAS_MEASURING").WithFlag(7, out new_data_0, FieldMode.Read, name: "NEW_DATA_0");
        }

        private State state;
        private Registers selectedRegister;
        private IValueRegisterField[] gasWait;
        private IValueRegisterField[] resHeat;
        private IValueRegisterField[] idacHeat;
        private IFlagRegisterField spi_3w_en;
        private IValueRegisterField filter;
        private IFlagRegisterField mode;
        private IValueRegisterField osrs_p;
        private IValueRegisterField osrs_t;
        private IFlagRegisterField spi_3w_int_en;
        private IValueRegisterField osrs_h;
        private IFlagRegisterField heat_off;
        private IFlagRegisterField run_gas;
        private IValueRegisterField nb_conv;
        private IValueRegisterField gas_range_r;
        private IValueRegisterField[] gas_r; // 0: lsb, 1: msb
        private IFlagRegisterField gas_valid_r;
        private IFlagRegisterField heat_stab_r;
        private IFlagRegisterField spi_mem_page;
        private IValueRegisterField[] hum; // 0: lsb, 1: msb
        private IValueRegisterField[] temp; // 0: xlsb, 1: lsb, 2: msb
        private IValueRegisterField[] press; // 0: xlsb, 1: lsb, 2: msb
        private IFlagRegisterField new_data_0;
        private IFlagRegisterField gas_measuring;
        private IFlagRegisterField measuring;
        private IValueRegisterField gas_meas_index;
        private const byte DEVICE_ID = 0x61;
        private const byte RESET_DATA = 0xb6;
        private const byte DEFAULT_DATA = 0x0;

        private enum Registers {
            EasStatus0 = 0x1d,
            PressMsb = 0x1f,
            PressLsb = 0x20,
            PressXlsb = 0x21,
            TempMsb = 0x22,
            TempLsb = 0x23,
            TempXlsb = 0x24,
            HumMsb = 0x25,
            HumLsb = 0x26,
            GasResMsb = 0x2a,
            GasResLsb = 0x2b,
            IdacHeat0 = 0x50,
            IdacHeat1 = 0x51,
            IdacHeat2 = 0x52,
            IdacHeat3 = 0x53,
            IdacHeat4 = 0x54,
            IdacHeat5 = 0x55,
            IdacHeat6 = 0x56,
            IdacHeat7 = 0x57,
            IdacHeat8 = 0x58,
            IdacHeat9 = 0x59,
            ResHeat0 = 0x5a,
            ResHeat1 = 0x5b,
            ResHeat2 = 0x5c,
            ResHeat3 = 0x5d,
            ResHeat4 = 0x5e,
            ResHeat5 = 0x5f,
            ResHeat6 = 0x60,
            ResHeat7 = 0x61,
            ResHeat8 = 0x62,
            ResHeat9 = 0x63,
            GasWait0 = 0x64,
            GasWait1 = 0x65,
            GasWait2 = 0x66,
            GasWait3 = 0x67,
            GasWait4 = 0x68,
            GasWait5 = 0x69,
            GasWait6 = 0x6a,
            GasWait7 = 0x6b,
            GasWait8 = 0x6c,
            GasWait9 = 0x6d,
            CtrlGas0 = 0x70,
            CtrlGas1 = 0x71,
            CtrlHum = 0x72,
            Status = 0x73,
            CtrlMeas = 0x74,
            Config = 0x75,
            Coeff1 = 0x8a,
            Coeff2 = 0xe1,
            Coeff3 = 0x00,
            Id = 0xd0,
            Reset = 0xe0,
        }

        private enum State {
            Idle,
            ReceivedFirstByte,
            WaitingForAddress,
            WritingWaitingForValue,
            Reading
        }
    }
}
