#ifndef PACKETPP_LAYER
#define PACKETPP_LAYER

#include <stdint.h>
#include <stdio.h>
#include "ProtocolType.h"

class Packet;

class Layer {
	friend class Packet;
public:
	virtual ~Layer() { if (!isAllocatedToPacket()) delete m_Data; }

	inline Layer* getNextLayer() { return m_NextLayer; }
	inline Layer* getPrevLayer() { return m_PrevLayer; }
	inline ProtocolType getProtocol() { return m_Protocol; }
	inline size_t getDataLen() { return m_DataLen; }

	inline bool isAllocatedToPacket() { return m_Packet != NULL; }

	void copyData(uint8_t* toArr);

	// abstract methods
	virtual void parseNextLayer() = 0;
	virtual size_t getHeaderLen() = 0;
	virtual void computeCalculateFields() = 0;

protected:
	uint8_t* m_Data;
	size_t m_DataLen;
	Packet* m_Packet;
	ProtocolType m_Protocol;
	Layer* m_NextLayer;
	Layer* m_PrevLayer;

	Layer() : m_Data(NULL), m_DataLen(0), m_Packet(NULL), m_Protocol(Unknown), m_NextLayer(NULL), m_PrevLayer(NULL) { }

	Layer(uint8_t* data, size_t dataLen, Layer* prevLayer, Packet* packet) :
		m_Data(data), m_DataLen(dataLen),
		m_Packet(packet), m_Protocol(Unknown),
		m_NextLayer(NULL), m_PrevLayer(prevLayer) {}

	inline void setNextLayer(Layer* nextLayer) { m_NextLayer = nextLayer; }
	inline void setPrevLayer(Layer* prevLayer) { m_PrevLayer = prevLayer; }

	virtual bool extendLayer(int offsetInLayer, size_t numOfBytesToExtend);
	virtual bool shortenLayer(int offsetInLayer, size_t numOfBytesToShorten);

private:
	// can't use copy c'tor and assignment operator
	Layer(const Layer& other );
	Layer& operator=(const Layer& other);
};

#endif /* PACKETPP_LAYER */