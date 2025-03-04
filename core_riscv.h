/*!****************************************************************************
 * @file
 * core_riscv.h
 *
 * @brief
 * RISC-V2A Core Peripheral Access
 *
 * @date  08.08.2020
 * @date  20.03.2023  Rewrite
 * @date  21.03.2023  Added remaining CSR access functions
 ******************************************************************************/

#ifndef CORE_RISCV_H_
#define CORE_RISCV_H_

/* ############################### Common ################################### */
/* Core version identifier: V2A                                               */
#define __RISC_V                      0x201U
#define __RISC_V2A                    1U

/* Volatile/Const Access flag definitions                                     */
#define __I                           volatile const
#define __O                           volatile
#define __IO                          volatile

/* Hint attributes for inlining functions                                     */
#define RV_STATIC_INLINE              static inline
#define RV_STATIC_FORCE_INLINE        RV_STATIC_INLINE __attribute__((always_inline))

/* Hint attributes for Hardware Prologue/Epilogue usage                       */
#ifdef USE_WCH_INTERRUPT_FAST_ATTR
#define RV_INTERRUPT __attribute__((interrupt("WCH-Interrupt-fast")))
#elif defined(USE_INTERRUPT_NAKED_ATTR)
#define RV_INTERRUPT __attribute__((naked))
#else
#define RV_INTERRUPT __attribute__((interrupt))
#endif /* USE_WCH_INTERRUPT_FAST_ATTR || USE_INTERRUPT_NAKED_ATTR */

/* Legacy support integer type definitions                                    */
typedef __I uint32_t vuc32;  /* Read Only */
typedef __I uint16_t vuc16;  /* Read Only */
typedef __I uint8_t vuc8;   /* Read Only */

typedef const uint32_t uc32;  /* Read Only */
typedef const uint16_t uc16;  /* Read Only */
typedef const uint8_t uc8;   /* Read Only */

typedef __I int32_t vsc32;  /* Read Only */
typedef __I int16_t vsc16;  /* Read Only */
typedef __I int8_t vsc8;   /* Read Only */

typedef const int32_t sc32;  /* Read Only */
typedef const int16_t sc16;  /* Read Only */
typedef const int8_t sc8;   /* Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

/* Error, function and status flag definitions                                */
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;


/* Core Peripheral Base Address definitions                                   */
#define PFIC_BASE                     0xE000E000UL
#define SysTick_BASE                  0xE000F000UL


/* ################ Programmable Fast Interrupt Controller ################## */
/* Memory-Mapped PFIC Register File                                           */
typedef struct{
  __I  uint32_t ISR[8];
  __I  uint32_t IPR[8];
  __IO uint32_t ITHRESDR;
  __IO uint32_t RESERVED;
  __IO uint32_t CFGR;
  __I  uint32_t GISR;
  __IO uint32_t VTFIDR;
       uint8_t  RESERVED0[12];
  __IO uint32_t VTFADDRR[4];
       uint8_t  RESERVED1[0x90];
  __O  uint32_t IENR[8];
       uint8_t  RESERVED2[0x60];
  __O  uint32_t IRER[8];
       uint8_t  RESERVED3[0x60];
  __O  uint32_t IPSR[8];
       uint8_t  RESERVED4[0x60];
  __O  uint32_t IPRR[8];
       uint8_t  RESERVED5[0x60];
  __IO uint32_t IACTR[8];
       uint8_t  RESERVED6[0xE0];
  __IO uint32_t IPRIOR[64];
       uint8_t  RESERVED7[0x810];
  __IO uint32_t SCTLR;
} PFIC_Type;

/* PFIC peripheral access macro                                               */
#define PFIC                          ((PFIC_Type*)PFIC_BASE)

/* Bit definitions for PFIC Configuration Register (CFGR)                     */
#define PFIC_CFGR_RESETSYS            0x00000080UL
#define PFIC_CFGR_KEYCODE_KEY1        0xFA050000UL
#define PFIC_CFGR_KEYCODE_KEY2        0xBCAF0000UL
#define PFIC_CFGR_KEYCODE_KEY3        0xBEEF0000UL

/* Bit definitions for PFIC System Control Register (SCTLR)                   */
#define PFIC_SCTLR_SLEEPONEXIT        0x00000002UL
#define PFIC_SCTLR_SLEEPDEEP          0x00000004UL
#define PFIC_SCTLR_WFITOWFE           0x00000008UL
#define PFIC_SCTLR_SEVONPEND          0x00000010UL
#define PFIC_SCTLR_SETEVENT           0x00000020UL
#define PFIC_SCTLR_SYSRESET           0x80000000UL

/* Bit definitions for PFIC VTF ID Configuration Register (VTFIDR)            */
#define PFIC_VTFIDR_VTFID             0x000000FFUL

/* Bit definitions for PFIC VTF Address Register (VTFADDRR)                   */
#define PFIC_VTFADDRR_ADDR            0xFFFFFFFEUL
#define PFIC_VTFADDRR_VTFEN           0x00000001UL

/* Bit definitions for PFIC Interrupt Priority Conf. Register (IPRIOR)        */
#define PFIC_IPRIOR_PRIO              0x000000FFUL

/* Interrupt number from IRQn                                                 */
#define PFIC_IRQn_NUM(IRQn)           ((uint32_t)(IRQn) & 0x1FUL)

/* Interrupt register offset from IRQn                                        */
#define PFIC_IRQn_REG(IRQn)           ((uint32_t)(IRQn) >> 5)

/* Interrupt System Control Register (INTSYSCR)                               */
#define PFIC_INTSYSCR_HWSTKEN         0x00000001UL
#define PFIC_INTSYSCR_INESTEN         0x00000002UL
#define PFIC_INTSYSCR_EABIEN          0x00000004UL

/*!****************************************************************************
 * @brief
 * Enable Interrupt
 *
 * @param[in] IRQn        Interrupt Number
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE void PFIC_EnableIRQ(IRQn_Type IRQn)
{
  PFIC->IENR[PFIC_IRQn_REG(IRQn)] = 1UL << PFIC_IRQn_NUM(IRQn);
}

/*!****************************************************************************
 * @brief
 * Disable Interrupt
 *
 * @param[in] IRQn        Interrupt Number
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE void PFIC_DisableIRQ(IRQn_Type IRQn)
{
  PFIC->IRER[PFIC_IRQn_REG(IRQn)] = 1UL << PFIC_IRQn_NUM(IRQn);
}

/*!****************************************************************************
 * @brief
 * Get Interrupt Enable State
 *
 * @param[in] IRQn        Interrupt Number
 * @return  (uint32_t)  Interrupt Enable State
 * @retval  0           Interrupt is disabled
 * @retval  1           Interrupt is enabled
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE uint32_t PFIC_GetStatusIRQ(IRQn_Type IRQn)
{
  return (PFIC->ISR[PFIC_IRQn_REG(IRQn)] & (1UL << PFIC_IRQn_NUM(IRQn))) ? 1UL : 0UL;
}

/*!****************************************************************************
 * @brief
 * Get Interrupt Pending State
 *
 * @param[in] IRQn        Interrupt Number
 * @return  (uint32_t)  Interrupt Pending State
 * @retval  0           Interrupt is not pending
 * @retval  1           Interrupt is pending
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE uint32_t PFIC_GetPendingIRQ(IRQn_Type IRQn)
{
  return (PFIC->IPR[PFIC_IRQn_REG(IRQn)] & (1UL << PFIC_IRQn_NUM(IRQn))) ? 1UL : 0UL;
}

/*!****************************************************************************
 * @brief
 * Set Interrupt Pending State
 *
 * @param[in] IRQn        Interrupt Number
 * @date  30.04.2020
 * @date  26.02.2023  Code Style
 ******************************************************************************/
RV_STATIC_INLINE void PFIC_SetPendingIRQ(IRQn_Type IRQn)
{
  PFIC->IPSR[PFIC_IRQn_REG(IRQn)] = 1UL << PFIC_IRQn_NUM(IRQn);
}

/*!****************************************************************************
 * @brief
 * Clear Interrupt Pending State
 *
 * @param[in] IRQn        Interrupt Number
 * @date  30.04.2020
 * @date  26.02.2023  Code Style
 ******************************************************************************/
RV_STATIC_INLINE void PFIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  PFIC->IPRR[PFIC_IRQn_REG(IRQn)] = 1UL << PFIC_IRQn_NUM(IRQn);
}

/*!****************************************************************************
 * @brief
 * Get Interrupt Active State
 *
 * @param[in] IRQn        Interrupt Number
 * @return  (uint32_t)  Interrupt Active State
 * @retval  0           Interrupt is not active
 * @retval  1           Interrupt is active
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE uint32_t PFIC_GetActive(IRQn_Type IRQn)
{
  return (PFIC->IACTR[PFIC_IRQn_REG(IRQn)] & (1UL << PFIC_IRQn_NUM(IRQn))) ? 1UL : 0UL;
}

/*!****************************************************************************
 * @brief
 * Set Interrupt Priority
 *
 * @param[in] IRQn        Interrupt Number
 * @param[in] priority    Priority value (7: pre-emption, 6: subpriority)
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE void PFIC_SetPriority(IRQn_Type IRQn, uint8_t priority)
{
  register uint32_t ulTmp = PFIC->IPRIOR[(IRQn >> 2)];
  register uint8_t ucShf = (IRQn & 0x3UL) << 3;

  ulTmp &= ~(PFIC_IPRIOR_PRIO << ucShf);
  ulTmp |= (uint32_t)(priority & 0xC0UL) << ucShf;

  PFIC->IPRIOR[(IRQn >> 2)] = ulTmp;
}

/*!****************************************************************************
 * @brief
 * Configure Vector-Table-Free (VTF) Interrupt Handler
 *
 * @note
 * Interrupt starts off in disabled state
 *
 * @param[in] channel     VTF Interrupt Channel (0..3)
 * @param[in] address     VTF Interrupt Handler Address
 * @param[in] IRQn        Assigned Interrupt Number
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE void PFIC_ConfigFastIRQ(uint8_t channel, uint32_t address, IRQn_Type IRQn)
{
  register uint32_t ulTmp = PFIC->VTFIDR;
  register uint8_t ucShf = (channel & 0x3UL) << 3;

  ulTmp &= ~(PFIC_VTFIDR_VTFID << ucShf);
  ulTmp |= (uint32_t)(IRQn & PFIC_VTFIDR_VTFID) << ucShf;

  PFIC->VTFIDR = ulTmp;
  PFIC->VTFADDRR[(channel & 0x3UL)] = (address & PFIC_VTFADDRR_ADDR);
}

/*!****************************************************************************
 * @brief
 * Enable Vector-Table-Free (VTF) Interrupt Handler
 *
 * @param[in] channel     VTF Interrupt Channel (0..3)
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE void PFIC_EnableFastIRQ(uint8_t channel)
{
  PFIC->VTFADDRR[(channel & 0x3UL)] |= PFIC_VTFADDRR_VTFEN;
}

/*!****************************************************************************
 * @brief
 * Disable Vector-Table-Free (VTF) Interrupt Handler
 *
 * @param[in] channel     VTF Interrupt Channel (0..3)
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE void PFIC_DisableFastIRQ(uint8_t channel)
{
  PFIC->VTFADDRR[(channel & 0x3UL)] &= ~PFIC_VTFADDRR_VTFEN;
}

/*!****************************************************************************
 * @brief
 * Initiate System Reset
 *
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE void PFIC_SystemReset(void)
{
#ifndef USE_PFIC_SCTLR_SYSRESET
  PFIC->CFGR = PFIC_CFGR_KEYCODE_KEY3 | PFIC_CFGR_RESETSYS;
#else
  PFIC->SCTLR |= PFIC_SCTLR_SYSRESET;
#endif /* USE_PFIC_SCTLR_SYSRESET */
}

/* ########################### SysTick Timer ################################ */
/* Memory-Mapped SysTick Register File                                        */
typedef struct
{
  __IO uint32_t CTLR;
  __IO uint32_t SR;
  __IO uint32_t CNTR;
       uint32_t RESERVED0;
  __IO uint32_t CMPR;
       uint32_t RESERVED1;
} SysTick_Type;

/* SysTick access macro                                                       */
#define SysTick                       ((SysTick_Type*)SysTick_BASE)

/* Bit definitions for SysTick Control Register (CTLR)                        */
#define SYSTICK_CTLR_STE              0x00000001UL
#define SYSTICK_CTLR_STIE             0x00000002UL
#define SYSTICK_CTLR_STCLK            0x00000004UL
#define SYSTICK_CTLR_STCLK_DIV8       0x00000000UL
#define SYSTICK_CTLR_STCLK_DIV1       SYSTICK_CTLR_STCLK
#define SYSTICK_CTLR_STRE             0x00000008UL
#define SYSTICK_CTLR_SWIE             0x80000000UL

/* Bit definitions for SysTick Status Register (SR)                           */
#define SYSTICK_SR_CNTIF              0x00000001UL

/*!****************************************************************************
 * @brief
 * Set SysTick counter value
 *
 * @param[in] value       Counter value
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE void SysTick_SetValue(uint32_t value)
{
  SysTick->CNTR = value;
}

/*!****************************************************************************
 * @brief
 * Get SysTick counter value
 *
 * @return  (uint32_t)  Counter value
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE uint32_t SysTick_GetValue(void)
{
  return SysTick->CNTR;
}

/*!****************************************************************************
 * @brief
 * Set SysTick compare value
 *
 * @param[in] value       Compare value
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE void SysTick_SetCompare(uint32_t value)
{
  SysTick->CMPR = value;
}

/*!****************************************************************************
 * @brief
 * Get SysTick compare value
 *
 * @return  (uint32_t)  Compare value
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_INLINE uint32_t SysTick_GetCompare(void)
{
  return SysTick->CMPR;
}


/* ############################ Core Functions ############################## */
/*!****************************************************************************
 * @brief
 * No Operation (NOP)
 *
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void __NOP()
{
  __asm volatile ("nop");
}

/*!****************************************************************************
 * @brief
 * Set Event Flag
 *
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void __SEV()
{
  PFIC->SCTLR |= PFIC_SCTLR_SETEVENT;
}

/*!****************************************************************************
 * @brief
 * Wait For Interrupt (WFI)
 *
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void __WFI(void)
{
  /* Instruction is executed as WFI                       */
  PFIC->SCTLR &= ~PFIC_SCTLR_WFITOWFE;
  __asm volatile ("wfi");
}

/*!****************************************************************************
 * @brief
 * Wait For Event (WFE)
 *
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void __WFE(void)
{
  /* Instruction is executed as WFE                       */
  PFIC->SCTLR |= PFIC_SCTLR_WFITOWFE;
  __asm volatile ("wfi");
}

/*!****************************************************************************
 * @brief
 * Signal Debugger Break (EBREAK)
 *
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void __EBREAK(void)
{
  __asm volatile ("ebreak");
}


/* ###################### Machine Register Access ########################### */
#define TEMPLATE_CSR_GETTER_FN(fname, csr)                                     \
/*!****************************************************************************
 * @brief
 * CSR Getter Function Template: uint32_t fname(void)
 *
 * @param[in] fname       (Template symbol) Function name
 * @param[in] csr         (Template string literal) CSR name
 * @return  (uint32_t)  CSR value
 * @date  21.03.2023
 *****************************************************************************/\
  uint32_t fname(void)                                                         \
  {                                                                            \
    uint32_t result;                                                           \
    __asm volatile ("csrr %0, " csr : "=r"(result));                           \
    return result;                                                             \
  }
#define TEMPLATE_CSR_SETTER_FN(fname, csr)                                     \
/*!****************************************************************************
 * @brief
 * CSR Setter Function Template: void fname(value)
 *
 * @param[in] fname       (Template symbol) Function name
 * @param[in] csr         (Template string literal) CSR name
 * @param[in] value       CSR value
 * @date  21.03.2023
 *****************************************************************************/\
  void fname(uint32_t value)                                                   \
  {                                                                            \
    __asm volatile ("csrw " csr ", %0" :: "r"(value));                         \
  }

/* Standard CSRs                                                              */
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MSTATUS,    "mstatus");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_MSTATUS,    "mstatus");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MISA,       "misa");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MIE,        "mie");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_MIE,        "mie");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MTVEC,      "mtvec");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_MTVEC,      "mtvec");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MSCRATCH,   "mscratch");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_MSCRATCH,   "mscratch");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MEPC,       "mepc");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_MEPC,       "mepc");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MCAUSE,     "mcause");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_MCAUSE,     "mcause");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MTVAL,      "mtval");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_MTVAL,      "mtval");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MIP,        "mip");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_MIP,        "mip");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MCYCLE,     "mcycle");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_MCYCLE,     "mcycle");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MCYCLEH,    "mcycleh");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_MCYCLEH,    "mcycleh");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MINSTRET,   "minstret");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_MINSTRET,   "minstret");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MINSTRETH,  "minstreth");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_MINSTRETH,  "minstreth");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MVENDORID,  "mvendorid");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MARCHID,    "marchid");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MIMPID,     "mimpid");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_MHARTID,    "mhartid");
/* WCH-custom CSRs                                                            */
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_DEBUGCR,    "0x7c0");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_DEBUGCR,    "0x7c0");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_GETTER_FN(__get_INTSYSCR,   "0x804");
RV_STATIC_FORCE_INLINE TEMPLATE_CSR_SETTER_FN(__set_INTSYSCR,   "0x804");


/* ############################## Other ##################################### */
/*!****************************************************************************
 * @brief
 * Get Stack Pointer value
 *
 * @return  (uint32_t)  SP register value
 * @date  30.04.2020
 * @date  26.02.2023
 ******************************************************************************/
RV_STATIC_FORCE_INLINE uint32_t __get_SP(void)
{
  uint32_t result;
  __asm volatile ("mv %0, sp" : "=r"(result));
  return result;
}

/*!****************************************************************************
 * @brief
 * Disable machine interrupts
 *
 * Clears MIE bit in mstatus register
 *
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void __disable_irq(void)
{
  __asm volatile ("csrci mstatus, 0x08" ::: "memory");
}

/*!****************************************************************************
 * @brief
 * Enable machine interrupts
 *
 * Sets MIE bit in mstatus register
 *
 * @date  21.03.2023
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void __enable_irq(void)
{
  __asm volatile ("csrsi mstatus, 0x08" ::: "memory");
}

/* ########################### PFIC (contd.) ################################ */
/*!****************************************************************************
 * @brief
 * Configure EABI support, HPE and Interrupt Nesting function
 *
 * @param[in] eabi        Enable or disable EABI support
 * @param[in] hpe         Enable or disable hardware prologue/epilogue (HPE)
 * @param[in] nest        Enable or disable interrupt nesting
 * @date  09.02.2025
 ******************************************************************************/
RV_STATIC_INLINE void PFIC_Config(FunctionalState eabi, FunctionalState hpe, FunctionalState nest)
{
  __set_INTSYSCR( \
    ((eabi != DISABLE) ? 0 : PFIC_INTSYSCR_EABIEN)  | \
    ((hpe != DISABLE)  ? 0 : PFIC_INTSYSCR_HWSTKEN) | \
    ((nest != DISABLE) ? 0 : PFIC_INTSYSCR_INESTEN));
}

#endif /* CORE_RISCV_H_ */
