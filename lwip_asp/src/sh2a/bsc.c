#include "bsc.h"
#include "iodefine.h"

void bsc_init(void)
{
	PORT.PCCR0.BIT.PC0MD = 1;	//~CS0
	PORT.PCCR0.BIT.PC1MD = 1;	//~RD
#ifndef BUS_16_BIT_BIG
	PORT.PCCR0.BIT.PC3MD = 1;	//~WE0/~DQML
#else
	PORT.PCCR1.BIT.PC4MD = 1;	//~WE1/~DQMU/~WE
#endif

#ifdef BUS_8_BIT
	PORT.PJCR0.BIT.PJ0MD = 5;	//A0
#endif
	PORT.PBCR0.BIT.PB1MD = 1;	//A1
	PORT.PBCR0.BIT.PB2MD = 1;	//A2
	PORT.PBCR0.BIT.PB3MD = 1;	//A3
	PORT.PBCR1.BIT.PB4MD = 1;	//A4
	PORT.PBCR1.BIT.PB5MD = 1;	//A5
#ifndef BUS_8_BIT
	PORT.PBCR1.BIT.PB6MD = 1;	//A6
#endif

	PORT.PDCR0.WORD = 0x1111;	//D0〜D3
	PORT.PDCR1.WORD = 0x1111;	//D4〜D7
#ifndef BUS_8_BIT
	PORT.PDCR2.WORD = 0x1111;	//D8〜D11
	PORT.PDCR3.WORD = 0x1111;	//D12〜D15
#endif

	BSC.CS0BCR.BIT.IWW		= 7;	//ライト-リード／ライト-ライトサイクル間アイドル指定
	BSC.CS0BCR.BIT.IWRWD	= 7;	//別空間リード-ライトサイクル間アイドル指定
	BSC.CS0BCR.BIT.IWRWS	= 7;	//同一空間リード-ライトサイクル間アイドル指定
	BSC.CS0BCR.BIT.IWRRD	= 7;	//別空間リード-リードサイクル間アイドル指定
	BSC.CS0BCR.BIT.IWRRS	= 7;	//同一空間リード-リードサイクル間アイドル指定
	BSC.CS0BCR.BIT.TYPE		= 0;	//接続メモリ種別：通常空間
#ifndef BUS_16_BIT_LITTLE
	BSC.CS0BCR.BIT.ENDIAN	= 0;	//エンディアン指定：ビッグエンディアンとして動作
#else
	BSC.CS0BCR.BIT.ENDIAN	= 1;	//エンディアン指定：リトルエンディアンとして動作
#endif
#ifdef BUS_8_BIT
	BSC.CS0BCR.BIT.BSZ		= 1;	//データバス幅指定：8ビット
#else
	BSC.CS0BCR.BIT.BSZ		= 2;	//データバス幅指定：16ビット
#endif

	BSC.CS0WCR.NORMAL.BIT.BAS	= 0;	//バイト選択つきSRAMバイトアクセス選択
	BSC.CS0WCR.NORMAL.BIT.SW	= 3;	//アドレス，~CS0アサート→~RD，~WEnアサート遅延サイクル数
	BSC.CS0WCR.NORMAL.BIT.WR	= 12;	//アクセスウェイトサイクル数
	BSC.CS0WCR.NORMAL.BIT.WM	= 1;	//外部ウェイトマスク指定：外部ウェイト入力無視
	BSC.CS0WCR.NORMAL.BIT.HW	= 3;	//~RD，~WEnネゲート→アドレス，~CS0ネゲート遅延サイクル数

	return;
}
