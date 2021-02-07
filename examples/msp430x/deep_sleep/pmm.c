/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// pmm.c - Driver for the pmm Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup pmm_api
//! @{
//
//*****************************************************************************

#include <io430f5359.h>
#include <stdint.h>
#include "tn_config.h"
#include "tn.h"
#include "types.h"
#include "pmm.h"

//*****************************************************************************
//
//! \brief Enables the low-side SVS circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_enableSvsL(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H)  = (unsigned char)0xA5;
        HWREG16(baseAddress + OFS_SVSMLCTL) |= (unsigned short)SVSLE;
        HWREG8(baseAddress + OFS_PMMCTL0_H)  = (unsigned char)0x00;
}

//*****************************************************************************
//
//! \brief Disables the low-side SVS circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_disableSvsL(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMLCTL) &= ~((unsigned short)SVSLE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables the low-side SVM circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_enableSvmL(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMLCTL) |= (unsigned short)SVMLE;
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables the low-side SVM circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_disableSvmL(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMLCTL) &= ~((unsigned short)SVMLE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables the high-side SVS circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_enableSvsH(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMHCTL) |= (unsigned short)SVSHE;
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables the high-side SVS circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_disableSvsH(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMHCTL) &= ~((unsigned short)SVSHE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables the high-side SVM circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_enableSvmH(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMHCTL) |= (unsigned short)SVMHE;
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables the high-side SVM circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_disableSvmH(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMHCTL) &= ~((unsigned short)SVMHE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables the low-side SVS and SVM circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_enableSvsLSvmL(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMLCTL) |= ((unsigned short)SVSLE + (unsigned short)SVMLE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables the low-side SVS and SVM circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_disableSvsLSvmL(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMLCTL) &= ~((unsigned short)SVSLE + (unsigned short)SVMLE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables the high-side SVS and SVM circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_enableSvsHSvmH(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMHCTL) |= ((unsigned short)SVSHE + (unsigned short)SVMHE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables the high-side SVS and SVM circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_disableSvsHSvmH(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMHCTL) &= ~((unsigned short)SVSHE + (unsigned short)SVMHE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables the POR signal generation when a low-voltage event is
//! registered by the low-side SVS
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
void PMM_enableSvsLReset(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_PMMRIE) |= (unsigned short)SVSLPE;
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables the POR signal generation when a low-voltage event is
//! registered by the low-side SVS
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
void PMM_disableSvsLReset(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_PMMRIE) &= ~((unsigned short)SVSLPE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables the interrupt generation when a low-voltage event is
//! registered by the low-side SVM
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
void PMM_enableSvmLInterrupt(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_PMMRIE) |= (unsigned short)SVMLIE;
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables the interrupt generation when a low-voltage event is
//! registered by the low-side SVM
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
void PMM_disableSvmLInterrupt(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_PMMRIE) &= ~((unsigned short)SVMLIE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables the POR signal generation when a low-voltage event is
//! registered by the high-side SVS
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
void PMM_enableSvsHReset(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_PMMRIE)  |= (unsigned short)SVSHPE;
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables the POR signal generation when a low-voltage event is
//! registered by the high-side SVS
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
void PMM_disableSvsHReset(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_PMMRIE) &= ~((unsigned short)SVSHPE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables the interrupt generation when a low-voltage event is
//! registered by the high-side SVM
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
void PMM_enableSvmHInterrupt(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_PMMRIE) |= (unsigned short)SVMHIE;
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables the interrupt generation when a low-voltage event is
//! registered by the high-side SVM
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
void PMM_disableSvmHInterrupt(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_PMMRIE) &= ~((unsigned short)SVMHIE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Clear all interrupt flags for the PMM
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIFG register.
//!
//! \return None
//
//*****************************************************************************
void PMM_clearPMMIFGS(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_PMMIFG) = 0;
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables supervisor low side in LPM with twake-up-fast from LPM2,
//! LPM3, and LPM4
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_SvsLEnabledInLPMFastWake(uint32_t baseAddress)
{
        //These settings use SVSH/LACE = 0
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMLCTL) |= ((unsigned short)SVSLFP + (unsigned short)SVSLMD);
        HWREG16(baseAddress + OFS_SVSMLCTL) &= ~((unsigned short)SVSMLACE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables supervisor low side in LPM with twake-up-slow from LPM2,
//! LPM3, and LPM4
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_SvsLEnabledInLPMSlowWake(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMLCTL) |= (unsigned short)SVSLMD;
        HWREG16(baseAddress + OFS_SVSMLCTL) &= ~((unsigned short)SVSLFP + (unsigned short)SVSMLACE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables supervisor low side in LPM with twake-up-fast from LPM2,
//! LPM3, and LPM4
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_SvsLDisabledInLPMFastWake(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMLCTL) |= (unsigned short)SVSLFP;
        HWREG16(baseAddress + OFS_SVSMLCTL) &= ~((unsigned short)SVSLMD + (unsigned short)SVSMLACE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables supervisor low side in LPM with twake-up-slow from LPM2,
//! LPM3, and LPM4
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_SvsLDisabledInLPMSlowWake(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMLCTL) &= ~((unsigned short)SVSLFP + 
                                                 (unsigned short)SVSMLACE + 
                                                 (unsigned short)SVSLMD);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables supervisor high side in LPM with tpd = 20 ?s(1)
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_SvsHEnabledInLPMNormPerf(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMHCTL) |= (unsigned short)SVSHMD;
        HWREG16(baseAddress + OFS_SVSMHCTL) &= ~((unsigned short)SVSMHACE + (unsigned short)SVSHFP);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Enables supervisor high side in LPM with tpd = 2.5 ?s(1)
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_SvsHEnabledInLPMFullPerf(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMHCTL) |= ((unsigned short)SVSHMD + (unsigned short)SVSHFP);
        HWREG16(baseAddress + OFS_SVSMHCTL) &= ~((unsigned short)SVSMHACE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables supervisor high side in LPM with tpd = 20 ?s(1)
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_SvsHDisabledInLPMNormPerf(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMHCTL) &= ~((unsigned short)SVSMHACE + 
                                                 (unsigned short)SVSHFP + 
                                                 (unsigned short)SVSHMD);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Disables supervisor high side in LPM with tpd = 2.5 ?s(1)
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_SvsHDisabledInLPMFullPerf(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMHCTL) |= (unsigned short)SVSHFP;
        HWREG16(baseAddress + OFS_SVSMHCTL) &= ~((unsigned short)SVSMHACE + (unsigned short)SVSHMD);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Optimized to provide twake-up-fast from LPM2, LPM3, and LPM4 with
//! least power
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_SvsLOptimizedInLPMFastWake(uint32_t baseAddress)
{
        //These setting use SVSH/LACE = 1
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMLCTL) |= ((unsigned short)SVSLFP + 
                                                (unsigned short)SVSLMD + 
                                                (unsigned short)SVSMLACE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Optimized to provide tpd = 2.5 ?s(1) in LPM with least power
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
void PMM_SvsHOptimizedInLPMFullPerf(uint32_t baseAddress)
{
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;
        HWREG16(baseAddress + OFS_SVSMHCTL) |= ((unsigned short)SVSHMD + 
                                                (unsigned short)SVSHFP + 
                                                (unsigned short)SVSMHACE);
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! \brief Increase Vcore by one level
//!
//! \param baseAddress is the base address of the PMM module.
//! \param level level to which Vcore needs to be increased
//!        Valid values are:
//!        - \b PMM_CORE_LEVEL_0 [Default]
//!        - \b PMM_CORE_LEVEL_1
//!        - \b PMM_CORE_LEVEL_2
//!        - \b PMM_CORE_LEVEL_3
//!
//! Modified bits of \b PMMCTL0 register, bits of \b PMMIFG register, bits of
//! \b PMMRIE register, bits of \b SVSMHCTL register and bits of \b SVSMLCTL
//! register.
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
uint16_t PMM_setVCoreUp(uint32_t baseAddress, uint8_t level)
{
        uint32_t PMMRIE_backup, SVSMHCTL_backup, SVSMLCTL_backup;
   int fExit;

        //The code flow for increasing the Vcore has been altered to work around
        //the erratum FLASH37.
        //Please refer to the Errata sheet to know if a specific device is affected
        //DO NOT ALTER THIS FUNCTION

        //Open PMM registers for write access
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;

        //Disable dedicated Interrupts
        //Backup all registers
        PMMRIE_backup = HWREG16(baseAddress + OFS_PMMRIE);
        HWREG16(baseAddress + OFS_PMMRIE) &= ~((unsigned short)((unsigned short)SVMHVLRPE | 
                                               (unsigned short)SVSHPE | 
                                               (unsigned short)SVMLVLRPE |
                                               (unsigned short)SVSLPE | 
                                               (unsigned short)SVMHVLRIE | 
                                               (unsigned short)SVMHIE |
                                               (unsigned short)SVSMHDLYIE | 
                                               (unsigned short)SVMLVLRIE | 
                                               (unsigned short)SVMLIE |
                                               (unsigned short)SVSMLDLYIE));
        SVSMHCTL_backup = HWREG16(baseAddress + OFS_SVSMHCTL);
        SVSMLCTL_backup = HWREG16(baseAddress + OFS_SVSMLCTL);

        //Clear flags
        HWREG16(baseAddress + OFS_PMMIFG) = 0;

        //Set SVM highside to new level and check if a VCore increase is possible
        HWREG16(baseAddress + OFS_SVSMHCTL) = (unsigned short)SVMHE | 
                                              (unsigned short)SVSHE | 
                                              ((unsigned short)SVSMHRRL0 * level);

        //Wait until SVM highside is settled
        while ((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMHDLYIFG) == 0U)
        {
        }

        //Clear flag
        HWREG16(baseAddress + OFS_PMMIFG) &= ~(unsigned short)SVSMHDLYIFG;

        //Check if a VCore increase is possible
        if ((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVMHIFG) == (unsigned short)SVMHIFG) 
        {
                //-> Vcc is too low for a Vcore increase
                //recover the previous settings
                HWREG16(baseAddress + OFS_PMMIFG) &= ~(unsigned short)SVSMHDLYIFG;
                HWREG16(baseAddress + OFS_SVSMHCTL) = (unsigned short)SVSMHCTL_backup;

                //Wait until SVM highside is settled
                while ((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMHDLYIFG) == 0U)
                {
                } 

                //Clear all Flags
                HWREG16(baseAddress + OFS_PMMIFG) &= ~((unsigned short)SVMHVLRIFG | 
                                                       (unsigned short)SVMHIFG | 
                                                       (unsigned short)SVSMHDLYIFG |
                                                       (unsigned short)SVMLVLRIFG | 
                                                       (unsigned short)SVMLIFG |
                                                       (unsigned short)SVSMLDLYIFG);

                //Restore PMM interrupt enable register
                HWREG16(baseAddress + OFS_PMMRIE) = (unsigned short)PMMRIE_backup;
                //Lock PMM registers for write access
                HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
                //return: voltage not set
                return STATUS_FAIL;
        }

        //Set also SVS highside to new level
        //Vcc is high enough for a Vcore increase
        HWREG16(baseAddress + OFS_SVSMHCTL) |= ((unsigned short)SVSHRVL0 * level);

        //Wait until SVM highside is settled
        while ((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMHDLYIFG) == 0U)
        {
        } 

        //Clear flag
        HWREG16(baseAddress + OFS_PMMIFG) &= ~(unsigned short)SVSMHDLYIFG;

        //Set VCore to new level
        HWREG8(baseAddress + OFS_PMMCTL0_L) = PMMCOREV0 * level;

        //Set SVM, SVS low side to new level
        HWREG16(baseAddress + OFS_SVSMLCTL) = (unsigned short)SVMLE | 
                                              ((unsigned short)SVSMLRRL0 * level) |
                                              (unsigned short)SVSLE | 
                                              ((unsigned short)SVSLRVL0 * level);

        //Wait until SVM, SVS low side is settled
        while ((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMLDLYIFG) == 0UL)
        {
        }

        //Clear flag
        HWREG16(baseAddress + OFS_PMMIFG) &= ~(unsigned short)SVSMLDLYIFG;
        //SVS, SVM core and high side are now set to protect for the new core level

        //Restore Low side settings
        //Clear all other bits _except_ level settings
        HWREG16(baseAddress + OFS_SVSMLCTL) &= ((unsigned short)SVSLRVL0 + 
                                                (unsigned short)SVSLRVL1 + 
                                                (unsigned short)SVSMLRRL0 +
                                                (unsigned short)SVSMLRRL1 + 
                                                (unsigned short)SVSMLRRL2);

        //Clear level settings in the backup register,keep all other bits
        SVSMLCTL_backup &=
                ~((unsigned short)SVSLRVL0 + (unsigned short)SVSLRVL1 + 
                  (unsigned short)SVSMLRRL0 + (unsigned short)SVSMLRRL1 + (unsigned short)SVSMLRRL2);

        //Restore low-side SVS monitor settings
        HWREG16(baseAddress + OFS_SVSMLCTL) |= (unsigned short)SVSMLCTL_backup;

        //Restore High side settings
        //Clear all other bits except level settings
        HWREG16(baseAddress + OFS_SVSMHCTL) &= ((unsigned short)SVSHRVL0 + 
                                                (unsigned short)SVSHRVL1 +
                                                (unsigned short)SVSMHRRL0 + 
                                                (unsigned short)SVSMHRRL1 +
                                                (unsigned short)SVSMHRRL2);

        //Clear level settings in the backup register,keep all other bits
        SVSMHCTL_backup &=
                ~((unsigned short)SVSHRVL0 + (unsigned short)SVSHRVL1 + 
                  (unsigned short)SVSMHRRL0 + (unsigned short)SVSMHRRL1 + 
                  (unsigned short)SVSMHRRL2);

        //Restore backup
        HWREG16(baseAddress + OFS_SVSMHCTL) |= (unsigned short)SVSMHCTL_backup;

        //Wait until high side, low side settled

     //   while (((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMLDLYIFG) == 0U) ||
     //          ((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMHDLYIFG) == 0U))
     //   {
     //   }  
          for(fExit = 0; fExit == 0;)
          {
             if((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMLDLYIFG) != 0U)
             {
                fExit = 1;  
             }

             if((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMHDLYIFG) != 0U)
             {
                fExit = 1;  
             }
          }
     //   {
     //   }  

        //Clear all Flags
        HWREG16(baseAddress + OFS_PMMIFG) &= ~((unsigned short)SVMHVLRIFG | 
                                               (unsigned short)SVMHIFG | 
                                               (unsigned short)SVSMHDLYIFG |
                                               (unsigned short)SVMLVLRIFG | 
                                               (unsigned short)SVMLIFG | 
                                               (unsigned short)SVSMLDLYIFG);

        //Restore PMM interrupt enable register
        HWREG16(baseAddress + OFS_PMMRIE) = (unsigned short)PMMRIE_backup;

        //Lock PMM registers for write access
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;

        return STATUS_SUCCESS;
}

//*****************************************************************************
//
//! \brief Decrease Vcore by one level
//!
//! \param baseAddress is the base address of the PMM module.
//! \param level level to which Vcore needs to be decreased
//!        Valid values are:
//!        - \b PMM_CORE_LEVEL_0 [Default]
//!        - \b PMM_CORE_LEVEL_1
//!        - \b PMM_CORE_LEVEL_2
//!        - \b PMM_CORE_LEVEL_3
//!
//! Modified bits of \b PMMCTL0 register, bits of \b PMMIFG register, bits of
//! \b PMMRIE register, bits of \b SVSMHCTL register and bits of \b SVSMLCTL
//! register.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
uint16_t PMM_setVCoreDown(uint32_t baseAddress, uint8_t level)
{
        uint32_t PMMRIE_backup, SVSMHCTL_backup, SVSMLCTL_backup;
   int fExit;

        //The code flow for decreasing the Vcore has been altered to work around
        //the erratum FLASH37.
        //Please refer to the Errata sheet to know if a specific device is affected
        //DO NOT ALTER THIS FUNCTION

        //Open PMM registers for write access
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0xA5;

        //Disable dedicated Interrupts
        //Backup all registers
        PMMRIE_backup = HWREG16(baseAddress + OFS_PMMRIE);
        HWREG16(baseAddress + OFS_PMMRIE) &= ~((unsigned short)SVMHVLRPE | 
                                               (unsigned short)SVSHPE | 
                                               (unsigned short)SVMLVLRPE |
                                               (unsigned short)SVSLPE | 
                                               (unsigned short)SVMHVLRIE | 
                                               (unsigned short)SVMHIE |
                                               (unsigned short)SVSMHDLYIE | 
                                               (unsigned short)SVMLVLRIE | 
                                               (unsigned short)SVMLIE |
                                               (unsigned short)SVSMLDLYIE);
        SVSMHCTL_backup = HWREG16(baseAddress + OFS_SVSMHCTL);
        SVSMLCTL_backup = HWREG16(baseAddress + OFS_SVSMLCTL);

        //Clear flags
        HWREG16(baseAddress + OFS_PMMIFG) &= ~((unsigned short)SVMHIFG | 
                                               (unsigned short)SVSMHDLYIFG |
                                               (unsigned short)SVMLIFG | 
                                               (unsigned short)SVSMLDLYIFG);

        //Set SVM, SVS high & low side to new settings in normal mode
        HWREG16(baseAddress + OFS_SVSMHCTL) = (unsigned short)SVMHE | ((unsigned short)SVSMHRRL0 * level) |
                                              (unsigned short)SVSHE | ((unsigned short)SVSHRVL0 * level);
        HWREG16(baseAddress + OFS_SVSMLCTL) = (unsigned short)SVMLE | ((unsigned short)SVSMLRRL0 * level) |
                                              (unsigned short)SVSLE | ((unsigned short)SVSLRVL0 * level);

        //Wait until SVM high side and SVM low side is settled
       // while ((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMHDLYIFG) == 0U ||
       //        (HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMLDLYIFG) == 0U) ;
        for(fExit = 0; fExit == 0;)
        {
           if((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMHDLYIFG) != 0U)
           {
              fExit =1;
           }
           if((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMLDLYIFG) != 0U)
           {
              fExit =1;
           }
        }


//        while ((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMHDLYIFG) == 0U ||
//               (HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMLDLYIFG) == 0U) ;
        for(fExit = 0; fExit == 0;)
        {
           if((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMHDLYIFG) != 0U)
           {
              fExit =1;
           }
           if((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMLDLYIFG) != 0U)
           {
              fExit =1;
           }
        }  

        //Clear flags
        HWREG16(baseAddress + OFS_PMMIFG) &= ~((unsigned short)SVSMHDLYIFG + (unsigned short)SVSMLDLYIFG);
        //SVS, SVM core and high side are now set to protect for the new core level

        //Set VCore to new level
        HWREG8(baseAddress + OFS_PMMCTL0_L) = (unsigned char)(PMMCOREV0 * level);

        //Restore Low side settings
        //Clear all other bits _except_ level settings
        HWREG16(baseAddress + OFS_SVSMLCTL) &= ((unsigned short)SVSLRVL0 + 
                                                (unsigned short)SVSLRVL1 + 
                                                (unsigned short)SVSMLRRL0 +
                                                (unsigned short)SVSMLRRL1 + 
                                                (unsigned short)SVSMLRRL2);

        //Clear level settings in the backup register,keep all other bits
        SVSMLCTL_backup &=
                ~((unsigned short)SVSLRVL0 + (unsigned short)SVSLRVL1 + 
                  (unsigned short)SVSMLRRL0 + (unsigned short)SVSMLRRL1 + 
                   (unsigned short)SVSMLRRL2);

        //Restore low-side SVS monitor settings
        HWREG16(baseAddress + OFS_SVSMLCTL) |= (unsigned short)SVSMLCTL_backup;

        //Restore High side settings
        //Clear all other bits except level settings
        HWREG16(baseAddress + OFS_SVSMHCTL) &= ((unsigned short)SVSHRVL0 + 
                                                (unsigned short)SVSHRVL1 + 
                                                (unsigned short)SVSMHRRL0 +
                                                (unsigned short)SVSMHRRL1 + 
                                                (unsigned short)SVSMHRRL2);

        //Clear level settings in the backup register, keep all other bits
        SVSMHCTL_backup &=
                ~((unsigned short)SVSHRVL0 + (unsigned short)SVSHRVL1 + 
                  (unsigned short)SVSMHRRL0 + (unsigned short)SVSMHRRL1 + 
                  (unsigned short)SVSMHRRL2);

        //Restore backup
        HWREG16(baseAddress + OFS_SVSMHCTL) |= (unsigned short)SVSMHCTL_backup;

        //Wait until high side, low side settled
        for(fExit = 0; fExit == 0;)
        {
           if((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMLDLYIFG) != 0U)
           {
              fExit =1;
           }

           if((HWREG16(baseAddress + OFS_PMMIFG) & (unsigned short)SVSMHDLYIFG) != 0U)
           {
              fExit =1;
           }
        }
        //Clear all Flags
        HWREG16(baseAddress + OFS_PMMIFG) &= ~((unsigned short)SVMHVLRIFG | 
                                                (unsigned short)SVMHIFG | 
                                                (unsigned short)SVSMHDLYIFG |
                                                (unsigned short)SVMLVLRIFG | 
                                                (unsigned short)SVMLIFG | 
                                                (unsigned short)SVSMLDLYIFG);

        //Restore PMM interrupt enable register
        HWREG16(baseAddress + OFS_PMMRIE) = (unsigned short)PMMRIE_backup;
        //Lock PMM registers for write access
        HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
        //Return: OK
        return STATUS_SUCCESS;
}

//*****************************************************************************
//
//! \brief Set Vcore to expected level
//!
//! \param baseAddress is the base address of the PMM module.
//! \param level level to which Vcore needs to be decreased/increased
//!        Valid values are:
//!        - \b PMM_CORE_LEVEL_0 [Default]
//!        - \b PMM_CORE_LEVEL_1
//!        - \b PMM_CORE_LEVEL_2
//!        - \b PMM_CORE_LEVEL_3
//!
//! Modified bits of \b PMMCTL0 register, bits of \b PMMIFG register, bits of
//! \b PMMRIE register, bits of \b SVSMHCTL register and bits of \b SVSMLCTL
//! register.
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
BOOL PMM_setVCore(uint32_t baseAddress, uint8_t level)
{
      /*  assert(
                (PMM_CORE_LEVEL_0 == level) ||
                (PMM_CORE_LEVEL_1 == level) ||
                (PMM_CORE_LEVEL_2 == level) ||
                (PMM_CORE_LEVEL_3 == level)
                );
      */
        uint8_t actlevel;
        BOOL status = STATUS_SUCCESS;

        //Set Mask for Max. level
        level &= PMMCOREV_3;

        //Get actual VCore
        actlevel = (unsigned char)(HWREG16(baseAddress + OFS_PMMCTL0) & PMMCOREV_3);

        //step by step increase or decrease
        while((level != actlevel) && (status == STATUS_SUCCESS)) 
        {
           if(level > actlevel)
           {  
              status = PMM_setVCoreUp(baseAddress, ++actlevel);
           }
           else
           {
              status = PMM_setVCoreDown(baseAddress, --actlevel);
           }
        }

        return status;
}

//*****************************************************************************
//
//! \brief Returns interrupt status
//!
//! \param baseAddress is the base address of the PMM module.
//! \param mask is the mask for specifying the required flag
//!        Mask value is the logical OR of any of the following:
//!        - \b PMM_SVSMLDLYIFG
//!        - \b PMM_SVMLIFG
//!        - \b PMM_SVMLVLRIFG
//!        - \b PMM_SVSMHDLYIFG
//!        - \b PMM_SVMHIFG
//!        - \b PMM_SVMHVLRIFG
//!        - \b PMM_PMMBORIFG
//!        - \b PMM_PMMRSTIFG
//!        - \b PMM_PMMPORIFG
//!        - \b PMM_SVSHIFG
//!        - \b PMM_SVSLIFG
//!        - \b PMM_PMMLPM5IFG
//!
//! \return Logical OR of any of the following:
//!         - \b PMM_SVSMLDLYIFG
//!         - \b PMM_SVMLIFG
//!         - \b PMM_SVMLVLRIFG
//!         - \b PMM_SVSMHDLYIFG
//!         - \b PMM_SVMHIFG
//!         - \b PMM_SVMHVLRIFG
//!         - \b PMM_PMMBORIFG
//!         - \b PMM_PMMRSTIFG
//!         - \b PMM_PMMPORIFG
//!         - \b PMM_SVSHIFG
//!         - \b PMM_SVSLIFG
//!         - \b PMM_PMMLPM5IFG
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
uint16_t PMM_getInterruptStatus(uint32_t baseAddress,
                                uint16_t mask)
{
        return (HWREG16(baseAddress + OFS_PMMIFG)) & mask;
}


//*****************************************************************************
//
//! Close the doxygen group for pmm_api
//! @}
//
//*****************************************************************************
