#pragma once

class CFusion
{
public:
	CFusion(void);
	~CFusion(void);

public:
	void Correlation_ImageFusion(LPBYTE lpMulspecDIBBits, LPBYTE lpSpotDIBBits, LPBYTE lpResultDIBBits,
		int lSpotHeight, int lSpotWidth, int lLineBytesMulspec, int lLineBytesSpot);
	void Brovey_ImageFusion(LPBYTE lpMulspecDIBBits, LPBYTE lpSpotDIBBits, LPBYTE lpFusionDIBBits,
		int lSpotHeight, int lSpotWidth, int lLineBytesMulspec, int lLineBytesSpot);
	void Mul_ImageFusion(LPBYTE lpMulspecDIBBits, LPBYTE lpSpotDIBBits, LPBYTE lpFusionDIBBits,
		int lSpotHeight, int lSpotWidth, int lLineBytesMulspec, int lLineBytesSpot);
	void LiearMul_ImageFusion(LPBYTE lpMulspecDIBBits, LPBYTE lpSpotDIBBits, LPBYTE lpFusionDIBBits,
		int lSpotHeight, int lSpotWidth, int lLineBytesMulspec, int lLineBytesSpot);
};
