#pragma once

#define RCNT0_COUNT			0x10000000
#define RCNT0_MODE			0x10000010
#define RCNT0_TARGET		0x10000020
#define RCNT0_HOLD			0x10000030
#define RCNT1_COUNT			0x10000800
#define RCNT1_MODE			0x10000810
#define RCNT1_TARGET		0x10000820
#define RCNT1_HOLD			0x10000830
#define RCNT2_COUNT			0x10001000
#define RCNT2_MODE			0x10001010
#define RCNT2_TARGET		0x10001020
#define RCNT3_COUNT			0x10001800
#define RCNT3_MODE			0x10001810
#define RCNT3_TARGET		0x10001820
#define IPU_CMD				0x10002000
#define IPU_CTRL			0x10002010
#define IPU_BP				0x10002020
#define IPU_TOP				0x10002030
#define GIF_CTRL			0x10003000
#define GIF_MODE			0x10003010
#define GIF_STAT			0x10003020
#define GIF_TAG0			0x10003040
#define GIF_TAG1			0x10003050
#define GIF_TAG2			0x10003060
#define GIF_TAG3			0x10003070
#define GIF_CNT				0x10003080
#define GIF_P3CNT			0x10003090
#define GIF_P3TAG			0x100030A0
#define VIF0_STAT		 	0x10003800
#define VIF0_FBRST		 	0x10003810
#define VIF0_ERR		 	0x10003820
#define VIF0_MARK		 	0x10003830
#define VIF0_CYCLE			0x10003840
#define VIF0_MODE		 	0x10003850
#define VIF0_NUM		 	0x10003860
#define VIF0_MASK		 	0x10003870
#define VIF0_CODE		 	0x10003880
#define VIF0_ITOPS			0x10003890
#define VIF0_ITOP		 	0x100038d0
#define VIF0_TOP			0x100038e0
#define VIF0_ROW0		 	0x10003900
#define VIF0_ROW1		 	0x10003910
#define VIF0_ROW2		 	0x10003920
#define VIF0_ROW3			0x10003930
#define VIF0_COL0		 	0x10003940
#define VIF0_COL1		 	0x10003950
#define VIF0_COL2			0x10003960
#define VIF0_COL3		 	0x10003970
#define VIF1_STAT			0x10003c00
#define VIF1_FBRST			0x10003c10
#define VIF1_ERR		 	0x10003c20
#define VIF1_MARK		 	0x10003c30
#define VIF1_CYCLE			0x10003c40
#define VIF1_MODE		 	0x10003c50
#define VIF1_NUM			0x10003c60
#define VIF1_MASK			0x10003c70
#define VIF1_CODE			0x10003c80
#define VIF1_ITOPS		 	0x10003c90
#define VIF1_BASE			0x10003ca0
#define VIF1_OFST		 	0x10003cb0
#define VIF1_TOPS		 	0x10003cc0
#define VIF1_ITOP		 	0x10003cd0
#define VIF1_TOP		 	0x10003ce0
#define VIF1_ROW0		 	0x10003d00
#define VIF1_ROW1		 	0x10003d10
#define VIF1_ROW2			0x10003d20
#define VIF1_ROW3			0x10003d30
#define VIF1_COL0			0x10003d40
#define VIF1_COL1			0x10003d50
#define VIF1_COL2		 	0x10003d60
#define VIF1_COL3		 	0x10003d70
#define VIF0_FIFO			0x10004000
#define VIF1_FIFO			0x10005000
#define GIF_FIFO			0x10006000
#define IPUout_FIFO			0x10007000
#define IPUin_FIFO			0x10007010
#define D0_CHCR				0x10008000
#define D0_MADR				0x10008010
#define D0_QWC				0x10008020
#define D0_TADR				0x10008030
#define D0_ASR0				0x10008040
#define D0_ASR1				0x10008050
#define VIF0_CHCR			0x10008000
#define VIF0_MADR			0x10008010
#define VIF0_QWC			0x10008020
#define VIF0_TADR			0x10008030
#define VIF0_ASR0			0x10008040
#define VIF0_ASR1			0x10008050
#define D1_CHCR				0x10009000
#define D1_MADR				0x10009010
#define D1_QWC				0x10009020
#define D1_TADR				0x10009030
#define D1_ASR0				0x10009040
#define D1_ASR1				0x10009050
#define VIF1_CHCR			0x10009000
#define VIF1_MADR			0x10009010
#define VIF1_QWC			0x10009020
#define VIF1_TADR			0x10009030
#define VIF1_ASR0			0x10009040
#define VIF1_ASR1			0x10009050
#define D2_CHCR				0x1000A000
#define D2_MADR				0x1000A010
#define D2_QWC				0x1000A020
#define D2_TADR				0x1000A030
#define D2_ASR0				0x1000A040
#define D2_ASR1				0x1000A050
#define GIF_CHCR			0x1000A000
#define GIF_MADR			0x1000A010
#define GIF_QWC				0x1000A020
#define GIF_TADR			0x1000A030
#define GIF_ASR0			0x1000A040
#define GIF_ASR1			0x1000A050
#define D3_CHCR				0x1000B000
#define D3_MADR				0x1000B010
#define D3_QWC				0x1000B020
#define fromIPU_CHCR		0x1000B000
#define fromIPU_MADR		0x1000B010
#define fromIPU_QWC			0x1000B020
#define D4_CHCR				0x1000B400
#define D4_MADR				0x1000B410
#define D4_QWC				0x1000B420
#define D4_TADR				0x1000B430
#define toIPU_CHCR			0x1000B400
#define toIPU_MADR			0x1000B410
#define toIPU_QWC			0x1000B420
#define toIPU_TADR			0x1000B430
#define D5_CHCR				0x1000C000
#define D5_MADR				0x1000C010
#define D5_QWC				0x1000C020
#define SIF0_CHCR			0x1000C000
#define SIF0_MADR			0x1000C010
#define SIF0_QWC			0x1000C020
#define D6_CHCR				0x1000C400
#define D6_MADR				0x1000C410
#define D6_QWC				0x1000C420
#define D6_TADR				0x1000C430
#define SIF1_CHCR			0x1000C400
#define SIF1_MADR			0x1000C410
#define SIF1_QWC			0x1000C420
#define SIF1_TADR			0x1000C430
#define D7_CHCR				0x1000C800
#define D7_MADR				0x1000C810
#define D7_QWC				0x1000C820
#define SIF2_CHCR			0x1000C800
#define SIF2_MADR			0x1000C810
#define SIF2_QWC			0x1000C820
#define D8_CHCR				0x1000D000
#define D8_MADR				0x1000D010
#define D8_QWC				0x1000D020
#define D8_SADR				0x1000D080
#define fromSPR_CHCR		0x1000D000
#define fromSPR_MADR		0x1000D010
#define fromSPR_QWC			0x1000D020
#define fromSPR_SADR		0x1000D080
#define D9_CHCR				0x1000D400
#define D9_MADR				0x1000D410
#define D9_QWC				0x1000D420
#define D9_TADR				0x1000D430
#define D9_SADR				0x1000D480
#define toSPR_CHCR			0x1000D400
#define toSPR_MADR			0x1000D410
#define toSPR_QWC			0x1000D420
#define toSPR_TADR			0x1000D430
#define toSPR_SADR			0x1000D480
#define DMAC_CTRL			0x1000E000
#define DMAC_STAT			0x1000E010
#define DMAC_PCR			0x1000E020
#define DMAC_SQWC			0x1000E030
#define DMAC_RBSR			0x1000E040
#define DMAC_RBOR			0x1000E050
#define DMAC_STADR			0x1000E060
#define DMAC_FAKESTAT		0x1000E100
#define INTC_STAT			0x1000F000
#define INTC_MASK			0x1000F010
#define SIO_LCR				0x1000F100
#define SIO_LSR				0x1000F110
#define SIO_IER				0x1000F120
#define SIO_ISR				0x1000F130
#define SIO_FCR				0x1000F140
#define SIO_BGR				0x1000F150
#define SIO_TXFIFO			0x1000F180
#define SIO_RXFIFO			0x1000F1C0
#define SBUS_F200			0x1000F200
#define SBUS_F210			0x1000F210
#define SBUS_F220			0x1000F220
#define SBUS_F230			0x1000F230
#define SBUS_F240			0x1000F240
#define SBUS_F250			0x1000F250
#define SBUS_F260			0x1000F260
#define SBUS_F300			0x1000F300
#define SBUS_F380			0x1000F380
#define MCH_RICM			0x1000F430
#define MCH_DRD				0x1000F440
#define DMAC_ENABLER		0x1000F520
#define DMAC_ENABLEW		0x1000F590

static char* regtostr(u32 addr) {
	switch(addr){
		case RCNT0_COUNT: return "RCNT0_COUNT"; break;
		case RCNT0_MODE: return "RCNT0_MODE"; break;
		case RCNT0_TARGET: return "RCNT0_TARGET"; break;
		case RCNT0_HOLD: return "RCNT0_HOLD"; break;
		case RCNT1_COUNT: return "RCNT1_COUNT"; break;
		case RCNT1_MODE: return "RCNT1_MODE"; break;
		case RCNT1_TARGET: return "RCNT1_TARGET"; break;
		case RCNT1_HOLD: return "RCNT1_HOLD"; break;
		case RCNT2_COUNT: return "RCNT2_COUNT"; break;
		case RCNT2_MODE: return "RCNT2_MODE"; break;
		case RCNT2_TARGET: return "RCNT2_TARGET"; break;
		case RCNT3_COUNT: return "RCNT3_COUNT"; break;
		case RCNT3_MODE: return "RCNT3_MODE"; break;
		case RCNT3_TARGET: return "RCNT3_TARGET"; break;
		case IPU_CMD: return "IPU_CMD"; break;
		case IPU_CTRL: return "IPU_CTRL"; break;
		case IPU_BP: return "IPU_BP"; break;
		case IPU_TOP: return "IPU_TOP"; break;
		case GIF_CTRL: return "GIF_CTRL"; break;
		case GIF_MODE: return "GIF_MODE"; break;
		case GIF_STAT: return "GIF_STAT"; break;
		case GIF_TAG0: return "GIF_TAG0"; break;
		case GIF_TAG1: return "GIF_TAG1"; break;
		case GIF_TAG2: return "GIF_TAG2"; break;
		case GIF_TAG3: return "GIF_TAG3"; break;
		case GIF_CNT: return "GIF_CNT"; break;
		case GIF_P3CNT: return "GIF_P3CNT"; break;
		case GIF_P3TAG: return "GIF_P3TAG"; break;
		case VIF0_STAT: return "VIF0_STAT"; break;
		case VIF0_FBRST: return "VIF0_FBRST"; break;
		case VIF0_ERR: return "VIF0_ERR"; break;
		case VIF0_MARK: return "VIF0_MARK"; break;
		case VIF0_CYCLE: return "VIF0_CYCLE"; break;
		case VIF0_MODE: return "VIF0_MODE"; break;
		case VIF0_NUM: return "VIF0_NUM"; break;
		case VIF0_MASK: return "VIF0_MASK"; break;
		case VIF0_CODE: return "VIF0_CODE"; break;
		case VIF0_ITOPS: return "VIF0_ITOPS"; break;
		case VIF0_ITOP: return "VIF0_ITOP"; break;
		case VIF0_TOP: return "VIF0_TOP"; break;
		case VIF0_ROW0: return "VIF0_ROW0"; break;
		case VIF0_ROW1: return "VIF0_ROW1"; break;
		case VIF0_ROW2: return "VIF0_ROW2"; break;
		case VIF0_ROW3: return "VIF0_ROW3"; break;
		case VIF0_COL0: return "VIF0_COL0"; break;
		case VIF0_COL1: return "VIF0_COL1"; break;
		case VIF0_COL2: return "VIF0_COL2"; break;
		case VIF0_COL3: return "VIF0_COL3"; break;
		case VIF1_STAT: return "VIF1_STAT"; break;
		case VIF1_FBRST: return "VIF1_FBRST"; break;
		case VIF1_ERR: return "VIF1_ERR"; break;
		case VIF1_MARK: return "VIF1_MARK"; break;
		case VIF1_CYCLE: return "VIF1_CYCLE"; break;
		case VIF1_MODE: return "VIF1_MODE"; break;
		case VIF1_NUM: return "VIF1_NUM"; break;
		case VIF1_MASK: return "VIF1_MASK"; break;
		case VIF1_CODE: return "VIF1_CODE"; break;
		case VIF1_ITOPS: return "VIF1_ITOPS"; break;
		case VIF1_BASE: return "VIF1_BASE"; break;
		case VIF1_OFST: return "VIF1_OFST"; break;
		case VIF1_TOPS: return "VIF1_TOPS"; break;
		case VIF1_ITOP: return "VIF1_ITOP"; break;
		case VIF1_TOP: return "VIF1_TOP"; break;
		case VIF1_ROW0: return "VIF1_ROW0"; break;
		case VIF1_ROW1: return "VIF1_ROW1"; break;
		case VIF1_ROW2: return "VIF1_ROW2"; break;
		case VIF1_ROW3: return "VIF1_ROW3"; break;
		case VIF1_COL0: return "VIF1_COL0"; break;
		case VIF1_COL1: return "VIF1_COL1"; break;
		case VIF1_COL2: return "VIF1_COL2"; break;
		case VIF1_COL3: return "VIF1_COL3"; break;
		case VIF0_FIFO: return "VIF0_FIFO"; break;
		case VIF1_FIFO: return "VIF1_FIFO"; break;
		case GIF_FIFO: return "GIF_FIFO"; break;
		case IPUout_FIFO: return "IPUout_FIFO"; break;
		case IPUin_FIFO: return "IPUin_FIFO"; break;
		case D0_CHCR: return "D0_CHCR"; break;
		case D0_MADR: return "D0_MADR"; break;
		case D0_QWC: return "D0_QWC"; break;
		case D0_TADR: return "D0_TADR"; break;
		case D0_ASR0: return "D0_ASR0"; break;
		case D0_ASR1: return "D0_ASR1"; break;
		//case VIF0_CHCR: return "VIF0_CHCR"; break;
		//case VIF0_MADR: return "VIF0_MADR"; break;
		//case VIF0_QWC: return "VIF0_QWC"; break;
		//case VIF0_TADR: return "VIF0_TADR"; break;
		//case VIF0_ASR0: return "VIF0_ASR0"; break;
		//case VIF0_ASR1: return "VIF0_ASR1"; break;
		case D1_CHCR: return "D1_CHCR"; break;
		case D1_MADR: return "D1_MADR"; break;
		case D1_QWC: return "D1_QWC"; break;
		case D1_TADR: return "D1_TADR"; break;
		case D1_ASR0: return "D1_ASR0"; break;
		case D1_ASR1: return "D1_ASR1"; break;
		//case VIF1_CHCR: return "VIF1_CHCR"; break;
		//case VIF1_MADR: return "VIF1_MADR"; break;
		//case VIF1_QWC: return "VIF1_QWC"; break;
		//case VIF1_TADR: return "VIF1_TADR"; break;
		//case VIF1_ASR0: return "VIF1_ASR0"; break;
		//case VIF1_ASR1: return "VIF1_ASR1"; break;
		case D2_CHCR: return "D2_CHCR"; break;
		case D2_MADR: return "D2_MADR"; break;
		case D2_QWC: return "D2_QWC"; break;
		case D2_TADR: return "D2_TADR"; break;
		case D2_ASR0: return "D2_ASR0"; break;
		case D2_ASR1: return "D2_ASR1"; break;
		//case GIF_CHCR: return "GIF_CHCR"; break;
		//case GIF_MADR: return "GIF_MADR"; break;
		//case GIF_QWC: return "GIF_QWC"; break;
		//case GIF_TADR: return "GIF_TADR"; break;
		//case GIF_ASR0: return "GIF_ASR0"; break;
		//case GIF_ASR1: return "GIF_ASR1"; break;
		case D3_CHCR: return "D3_CHCR"; break;
		case D3_MADR: return "D3_MADR"; break;
		case D3_QWC: return "D3_QWC"; break;
		//case fromIPU_CHCR: return "fromIPU_CHCR"; break;
		//case fromIPU_MADR: return "fromIPU_MADR"; break;
		//case fromIPU_QWC: return "fromIPU_QWC"; break;
		case D4_CHCR: return "D4_CHCR"; break;
		case D4_MADR: return "D4_MADR"; break;
		case D4_QWC: return "D4_QWC"; break;
		case D4_TADR: return "D4_TADR"; break;
		//case toIPU_CHCR: return "toIPU_CHCR"; break;
		//case toIPU_MADR: return "toIPU_MADR"; break;
		//case toIPU_QWC: return "toIPU_QWC"; break;
		//case toIPU_TADR: return "toIPU_TADR"; break;
		case D5_CHCR: return "D5_CHCR"; break;
		case D5_MADR: return "D5_MADR"; break;
		case D5_QWC: return "D5_QWC"; break;
		//case SIF0_CHCR: return "SIF0_CHCR"; break;
		//case SIF0_MADR: return "SIF0_MADR"; break;
		//case SIF0_QWC: return "SIF0_QWC"; break;
		case D6_CHCR: return "D6_CHCR"; break;
		case D6_MADR: return "D6_MADR"; break;
		case D6_QWC: return "D6_QWC"; break;
		case D6_TADR: return "D6_TADR"; break;
		//case SIF1_CHCR: return "SIF1_CHCR"; break;
		//case SIF1_MADR: return "SIF1_MADR"; break;
		//case SIF1_QWC: return "SIF1_QWC"; break;
		//case SIF1_TADR: return "SIF1_TADR"; break;
		case D7_CHCR: return "D7_CHCR"; break;
		case D7_MADR: return "D7_MADR"; break;
		case D7_QWC: return "D7_QWC"; break;
		//case SIF2_CHCR: return "SIF2_CHCR"; break;
		//case SIF2_MADR: return "SIF2_MADR"; break;
		//case SIF2_QWC: return "SIF2_QWC"; break;
		case D8_CHCR: return "D8_CHCR"; break;
		case D8_MADR: return "D8_MADR"; break;
		case D8_QWC: return "D8_QWC"; break;
		case D8_SADR: return "D8_SADR"; break;
		//case fromSPR_CHCR: return "fromSPR_CHCR"; break;
		//case fromSPR_MADR: return "fromSPR_MADR"; break;
		//case fromSPR_QWC: return "fromSPR_QWC"; break;
		//case fromSPR_SADR: return "fromSPR_SADR"; break;
		case D9_CHCR: return "D9_CHCR"; break;
		case D9_MADR: return "D9_MADR"; break;
		case D9_QWC: return "D9_QWC"; break;
		case D9_TADR: return "D9_TADR"; break;
		case D9_SADR: return "D9_SADR"; break;
		//case toSPR_CHCR: return "toSPR_CHCR"; break;
		//case toSPR_MADR: return "toSPR_MADR"; break;
		//case toSPR_QWC: return "toSPR_QWC"; break;
		//case toSPR_TADR: return "toSPR_TADR"; break;
		//case toSPR_SADR: return "toSPR_SADR"; break;
		case DMAC_CTRL: return "DMAC_CTRL"; break;
		case DMAC_STAT: return "DMAC_STAT"; break;
		case DMAC_PCR: return "DMAC_PCR"; break;
		case DMAC_SQWC: return "DMAC_SQWC"; break;
		case DMAC_RBSR: return "DMAC_RBSR"; break;
		case DMAC_RBOR: return "DMAC_RBOR"; break;
		case DMAC_STADR: return "DMAC_STADR"; break;
		case DMAC_FAKESTAT: return "DMAC_FAKESTAT"; break;
		case INTC_STAT: return "INTC_STAT"; break;
		case INTC_MASK: return "INTC_MASK"; break;
		case SIO_LCR: return "SIO_LCR"; break;
		case SIO_LSR: return "SIO_LSR"; break;
		case SIO_IER: return "SIO_IER"; break;
		case SIO_ISR: return "SIO_ISR"; break;
		case SIO_FCR: return "SIO_FCR"; break;
		case SIO_BGR: return "SIO_BGR"; break;
		case SIO_TXFIFO: return "SIO_TXFIFO"; break;
		case SIO_RXFIFO: return "SIO_RXFIFO"; break;
		case SBUS_F200: return "SBUS_F200"; break;
		case SBUS_F210: return "SBUS_F210"; break;
		case SBUS_F220: return "SBUS_F220"; break;
		case SBUS_F230: return "SBUS_F230"; break;
		case SBUS_F240: return "SBUS_F240"; break;
		case SBUS_F250: return "SBUS_F250"; break;
		case SBUS_F260: return "SBUS_F260"; break;
		case SBUS_F300: return "SBUS_F300"; break;
		case SBUS_F380: return "SBUS_F380"; break;
		case MCH_RICM: return "MCH_RICM"; break;
		case MCH_DRD: return "MCH_DRD"; break;
		case DMAC_ENABLER: return "DMAC_ENABLER"; break;
		case DMAC_ENABLEW: return "DMAC_ENABLEW"; break;
		}

	return "invalid reg";
}