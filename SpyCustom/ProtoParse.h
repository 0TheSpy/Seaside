#pragma once
#include <cstdint>
#include <string>
#include <vector>

#define MAKE_TAG(FIELD_NUMBER, TYPE) static_cast<uint32_t>(((FIELD_NUMBER) << kTagTypeBits) | (TYPE))

#define make_struct(_name_, _size_) \
	struct _name_ : ProtoWriter { \
		constexpr static size_t MAX_FIELD = _size_; \
		_name_()						: ProtoWriter(MAX_FIELD) {} \
		_name_(void* data, size_t size) : ProtoWriter(data, size, MAX_FIELD) {} \
		_name_(std::string data)        : ProtoWriter(data, MAX_FIELD) {}

#define make_field(_name_, _id_, _type_) \
	constexpr static Tag _name_ = { _id_, _type_ }; \
	void clear_##_name_() { this->clear(_name_); } \
	bool has_##_name_() { return this->has(_name_); } \
	Field get_##_name_() { return this->get(_name_); } \
	std::vector<Field> getAll_##_name_() { return this->getAll(_name_); } \
	\
	void add_##_name_(std::string v) { this->add(_name_, v); } \
	template<typename T> void add_##_name_(T v) { this->add(_name_, v); } \
	void replace_##_name_(std::string v) { this->replace(_name_, v); } \
	void replace_##_name_(std::string v, uint32_t index) { this->replace(_name_, v, index); } \
	template<typename T> void replace_##_name_(T v) { this->replace(_name_, v); } \
	template<typename T> void replace_##_name_(T v, uint32_t index) { this->replace(_name_, v, index); } \
	template<class T> T get_##_name_() { return std::move( T(this->get(_name_).String()) ); }


struct Tag
{
	unsigned field;
	unsigned type;
};

enum FieldType {
	TYPE_DOUBLE = 1,
	TYPE_FLOAT = 2,
	TYPE_INT64 = 3,
	TYPE_UINT64 = 4,
	TYPE_INT32 = 5,
	TYPE_FIXED64 = 6,
	TYPE_FIXED32 = 7,
	TYPE_BOOL = 8,
	TYPE_STRING = 9,
	TYPE_GROUP = 10,
	TYPE_MESSAGE = 11,
	TYPE_BYTES = 12,
	TYPE_UINT32 = 13,
	TYPE_ENUM = 14,
	TYPE_SFIXED32 = 15,
	TYPE_SFIXED64 = 16,
	TYPE_SINT32 = 17,
	TYPE_SINT64 = 18,
	MAX_FIELD_TYPE = 18,
};

struct Field
{
	friend class ProtoWriter;
public:
	inline Field& operator=(const Field& f);

	Field() : tag({ 0,0 }), value(""), full("") { }
	Field(unsigned field, unsigned type, std::string value, std::string full);

	Field(Tag tag, std::string value);
	Field(unsigned field, unsigned type, std::string value);

	template<typename T>
	Field(Tag tag, T value);

	template<typename T>
	Field(unsigned field, unsigned type, T value);

public:
	static Field ReadField(void* data, size_t& bytesRead);

public:
	inline float Float();
	inline double Double();
	inline int32_t Int32();
	inline int64_t Int64();
	inline uint32_t UInt32();
	inline uint64_t UInt64();
	inline uint32_t Fixed32();
	inline uint64_t Fixed64();
	inline int32_t SFixed32();
	inline int64_t SFixed64();
	inline bool Bool();
	inline std::string String();

private:
	Tag tag;
	std::string value;
	std::string full;

	static std::string getBytesVarint32(uint32_t value);
	static std::string getBytesVarint64(uint64_t value);
	static uint32_t readVarUint32(void* data, size_t& bytesRead);
	static uint64_t readVarUint64(void* data, size_t& bytesRead);

	enum WireType {
		WIRETYPE_VARINT = 0,
		WIRETYPE_FIXED64 = 1,
		WIRETYPE_LENGTH_DELIMITED = 2,
		WIRETYPE_START_GROUP = 3,
		WIRETYPE_END_GROUP = 4,
		WIRETYPE_FIXED32 = 5,
	};

	constexpr static WireType kWireTypeForFieldType[MAX_FIELD_TYPE + 1] = {
		static_cast<WireType>(-1),   
		WIRETYPE_FIXED64,            
		WIRETYPE_FIXED32,            
		WIRETYPE_VARINT,             
		WIRETYPE_VARINT,             
		WIRETYPE_VARINT,             
		WIRETYPE_FIXED64,            
		WIRETYPE_FIXED32,            
		WIRETYPE_VARINT,             
		WIRETYPE_LENGTH_DELIMITED,   
		WIRETYPE_START_GROUP,        
		WIRETYPE_LENGTH_DELIMITED,   
		WIRETYPE_LENGTH_DELIMITED,   
		WIRETYPE_VARINT,             
		WIRETYPE_VARINT,             
		WIRETYPE_FIXED32,            
		WIRETYPE_FIXED64,            
		WIRETYPE_VARINT,             
		WIRETYPE_VARINT,             
	};

	constexpr static int kTagTypeBits = 3;
	constexpr static uint32_t kTagTypeMask = (1 << kTagTypeBits) - 1;
	constexpr static int kMaxVarintBytes = 10;
	constexpr static int kMaxVarint32Bytes = 5;
};

class ProtoWriter
{
public:
	inline ProtoWriter();
	inline ProtoWriter(size_t maxFields);
	inline ProtoWriter(void* data, size_t size, size_t maxFields);
	inline ProtoWriter(std::string dataStr, size_t maxFields);

public:
	inline void add(Field field);
	inline void replace(Field field);
	inline void replace(Field field, uint32_t index);
	inline void clear(unsigned fieldId);
	inline bool has(unsigned fieldId);
	inline Field get(unsigned fieldId);
	inline std::vector<Field> getAll(unsigned fieldId);

public:
	inline void add(Tag tag, std::string value);
	inline void replace(Tag tag, std::string value);
	inline void replace(Tag tag, std::string value, uint32_t index);
	inline void clear(Tag tag);
	inline bool has(Tag tag);
	inline Field get(Tag tag);
	inline std::vector<Field> getAll(Tag tag);

	template<typename T>
	inline void add(Tag tag, T value);

	template<typename T>
	inline void replace(Tag tag, T value);

	template<typename T>
	inline void replace(Tag tag, T value, uint32_t index);


	std::string serialize();
	void print();

private:
	std::vector<std::vector<Field>> fields;
};

#pragma region Helper Functions
std::string Field::getBytesVarint32(uint32_t value)
{
	uint8_t bytes[kMaxVarint32Bytes];
	int size = 0;
	while (value > 0x7F) {
		bytes[size++] = (static_cast<uint8_t>(value) & 0x7F) | 0x80;
		value >>= 7;
	}
	bytes[size++] = static_cast<uint8_t>(value) & 0x7F;
	return std::string{ reinterpret_cast<const char*>(&bytes[0]), (size_t)size };
}

std::string Field::getBytesVarint64(uint64_t value)
{
	uint8_t bytes[kMaxVarintBytes];
	int size = 0;
	while (value > 0x7F) {
		bytes[size++] = (static_cast<uint8_t>(value) & 0x7F) | 0x80;
		value >>= 7;
	}
	bytes[size++] = static_cast<uint8_t>(value) & 0x7F;
	return std::string{ reinterpret_cast<const char*>(&bytes[0]), (size_t)size };
}

uint32_t Field::readVarUint32(void* data, size_t& bytesRead)
{
	auto ptr = reinterpret_cast<const uint8_t*>(data);
	auto value = 0u;
	auto bytes = 0u;

	do {
		value |= static_cast<uint32_t>(*ptr & 0x7f) << (7 * bytes);
		bytes++;
	} while (*(ptr++) & 0x80 && bytes <= 5);

	bytesRead = bytes;
	return value;
}

uint64_t Field::readVarUint64(void* data, size_t& bytesRead)
{
	auto ptr = reinterpret_cast<const uint8_t*>(data);
	auto value = 0ull;
	auto bytes = 0u;

	do
	{
		value |= static_cast<uint64_t>(*ptr & 0x7f) << (7 * bytes);
		bytes++;
	} while (*(ptr++) & 0x80 && bytes <= 10);

	bytesRead = bytes;

	return value;
}

Field Field::ReadField(void* data, size_t& bytesRead)
{
	unsigned field = *reinterpret_cast<uint16_t*>(data);
	unsigned type = field & kTagTypeMask;

	if (field == 0xffff) {
		bytesRead = 0;
		return Field();
	}

	if (field & 0x80) {
		field = ((field & 0x7f) | ((field & 0xff00) >> 1)) >> kTagTypeBits;
		bytesRead = 2;
	}
	else {
		field = (field & 0xff) >> kTagTypeBits;
		bytesRead = 1;
	}

	size_t length, sizeDelimited;
	std::string value, full;
	switch (type)
	{
	case WIRETYPE_VARINT:
		readVarUint64((void*)((ptrdiff_t)data + bytesRead), length);
		value = std::string{ reinterpret_cast<const char*>((void*)((ptrdiff_t)data + bytesRead)), length };
		full = std::string{ reinterpret_cast<const char*>(data), bytesRead + length };
		bytesRead += length;
		break;
	case WIRETYPE_FIXED64:
		value = std::string{ reinterpret_cast<const char*>((void*)((ptrdiff_t)data + bytesRead)), 8 };
		full = std::string{ reinterpret_cast<const char*>(data), bytesRead + 8 };
		bytesRead += 8;
		break;
	case WIRETYPE_LENGTH_DELIMITED:
		sizeDelimited = readVarUint32((void*)((ptrdiff_t)data + bytesRead), length);
		value = std::string{ reinterpret_cast<const char*>((void*)((ptrdiff_t)data + bytesRead)), length + sizeDelimited };
		full = std::string{ reinterpret_cast<const char*>(data), bytesRead + length + sizeDelimited };
		bytesRead += length + sizeDelimited;
		break;
	case WIRETYPE_START_GROUP:
		throw("WIRETYPE_START_GROUP unrealised");
		break;
	case WIRETYPE_END_GROUP:
		throw("WIRETYPE_END_GROUP unrealised");
		break;
	case WIRETYPE_FIXED32:
		value = std::string{ reinterpret_cast<const char*>((void*)((ptrdiff_t)data + bytesRead)), 4 };
		full = std::string{ reinterpret_cast<const char*>(data), bytesRead + 4 };
		bytesRead += 4;
		break;
	default:
		throw("Unknown type %i", type);
		break;
	}


	return Field(field, type, value, full);
}

#pragma endregion 

#pragma region Field Definition
Field& Field::operator=(const Field& f) {
	this->tag = f.tag;
	this->value = f.value;
	this->full = f.full;
	return *this;
}

Field::Field(unsigned field, unsigned type, std::string value, std::string full) {
	this->tag = { field, type };
	this->value = value;
	this->full = full;
}

template<typename T>
Field::Field(Tag tag, T value) {
	auto wireType = kWireTypeForFieldType[tag.type];
	full = getBytesVarint32(MAKE_TAG(tag.field, wireType));

	switch (wireType) {
	case WIRETYPE_VARINT:
		full += getBytesVarint64(static_cast<uint64_t>(value));
		break;
	case WIRETYPE_FIXED32:
		full += std::string{ reinterpret_cast<const char*>(&value), 4 };
		break;
	case WIRETYPE_FIXED64:
		full += std::string{ reinterpret_cast<const char*>(&value), 8 };
		break;
	}
}

template<typename T>
Field::Field(unsigned field, unsigned type, T value) {
	auto wireType = kWireTypeForFieldType[type];
	tag = { field, (unsigned)wireType };
	full = getBytesVarint32(MAKE_TAG(field, wireType));

	switch (wireType) {
	case WIRETYPE_VARINT:
		full += getBytesVarint64(static_cast<uint64_t>(value));
		break;
	case WIRETYPE_FIXED32:
		full += std::string{ reinterpret_cast<const char*>(&value), 4 };
		break;
	case WIRETYPE_FIXED64:
		full += std::string{ reinterpret_cast<const char*>(&value), 8 };
		break;
	}
}

Field::Field(Tag tag, std::string value) {
	auto wireType = kWireTypeForFieldType[tag.type];
	full = getBytesVarint32(MAKE_TAG(tag.field, wireType));
	full += getBytesVarint32(value.size());
	full += value;
}

Field::Field(unsigned field, unsigned type, std::string value) {
	auto wireType = kWireTypeForFieldType[type];
	tag = { field, (unsigned)wireType };
	full = getBytesVarint32(MAKE_TAG(field, wireType));
	full += getBytesVarint32(value.size());
	full += value;
}


float Field::Float() {
	return *reinterpret_cast<float*>((void*)value.data());
}
double Field::Double() {
	return *reinterpret_cast<double*>((void*)value.data());
}
int32_t Field::Int32() {
	size_t bytesRead;
	return static_cast<int32_t>(readVarUint64((void*)value.data(), bytesRead));
}
int64_t Field::Int64() {
	size_t bytesRead;
	return readVarUint64((void*)value.data(), bytesRead);
}
uint32_t Field::UInt32() {
	size_t bytesRead;
	return readVarUint32((void*)value.data(), bytesRead);
}
uint64_t Field::UInt64() {
	size_t bytesRead;
	return readVarUint64((void*)value.data(), bytesRead);
}
uint32_t Field::Fixed32() {
	return *reinterpret_cast<uint32_t*>((void*)value.data());
}
uint64_t Field::Fixed64() {
	return *reinterpret_cast<uint64_t*>((void*)value.data());
}
int32_t Field::SFixed32() {
	return *reinterpret_cast<int32_t*>((void*)value.data());
}
int64_t Field::SFixed64() {
	return *reinterpret_cast<int64_t*>((void*)value.data());
}
bool Field::Bool() {
	size_t bytesRead;
	return !!readVarUint32((void*)value.data(), bytesRead);
}

std::string Field::String()
{
	size_t bytesRead;
	void* data = (void*)value.data();
	auto length = readVarUint32((void*)value.data(), bytesRead);
	auto value = std::string{ reinterpret_cast<const char*>((void*)((ptrdiff_t)data + bytesRead)), length };
	return value;
}

#pragma endregion

#pragma region ProtoWriter Definition
ProtoWriter::ProtoWriter()
{

}

ProtoWriter::ProtoWriter(size_t maxFields)
{
	size_t vector_size = maxFields + 1;
	fields.resize(vector_size);
	fields.reserve(vector_size);
}

ProtoWriter::ProtoWriter(void* data, size_t size, size_t maxFields) : ProtoWriter(maxFields)
{
	size_t vector_size = maxFields + 1,
		pos = 0,
		bytesRead;

	if (data == nullptr)
		return;
	while (pos < size) {
		auto field = Field::ReadField((void*)((ptrdiff_t)data + pos), bytesRead);
		if (!bytesRead) break;

		auto index = field.tag.field;
		if (index >= vector_size) throw("fields range error: field[%i]", index);
		fields[index].push_back(field);
		pos += bytesRead;
	}
}

ProtoWriter::ProtoWriter(std::string dataStr, size_t maxFields) : ProtoWriter((void*)dataStr.data(), dataStr.size(), maxFields)
{

}

std::string ProtoWriter::serialize()
{
	std::string result;
	for (auto& f0 : fields) {
		for (auto& f1 : f0) {
			result += f1.full;
		}
	}
	return result;
}

void ProtoWriter::print()
{
	auto data = serialize();
	void* mem = (void*)data.data();
	size_t size = data.size();
	int j = 0;
	for (size_t i = 0; i <= size; ++i) {
		printf("%.2X ", *(unsigned char*)((uintptr_t)mem + i));
		j++;
		if (j == 16)
		{
			j = 0;
			printf("\n");
		}
	}
	printf("\n");
}

void ProtoWriter::add(Field field)
{
	fields[field.tag.field].push_back(field);
}

void ProtoWriter::replace(Field field)
{
	fields[field.tag.field].clear();
	fields[field.tag.field].push_back(field);
}

void ProtoWriter::replace(Field field, uint32_t index)
{
	fields[field.tag.field][index] = field;
}

void ProtoWriter::clear(unsigned fieldId)
{
	return fields[fieldId].clear();
}

bool ProtoWriter::has(unsigned fieldId)
{
	return fields[fieldId].size() > 0;
}

Field ProtoWriter::get(unsigned fieldId)
{
	if (fields[fieldId].empty())
		return Field();
	return fields[fieldId][0];
}

std::vector<Field> ProtoWriter::getAll(unsigned fieldId)
{
	return fields[fieldId];
}

template<typename T>
void ProtoWriter::add(Tag tag, T value)
{
	fields[tag.field].push_back(Field(tag, value));
}

template<typename T>
void ProtoWriter::replace(Tag tag, T value)
{
	fields[tag.field].clear();
	fields[tag.field].push_back(Field(tag, value));
}

template<typename T>
void ProtoWriter::replace(Tag tag, T value, uint32_t index)
{
	fields[tag.field][index] = Field(tag, value);
}

void ProtoWriter::add(Tag tag, std::string value)
{
	fields[tag.field].push_back(Field(tag, value));
}

void ProtoWriter::replace(Tag tag, std::string value)
{
	fields[tag.field].clear();
	fields[tag.field].push_back(Field(tag, value));
}

void ProtoWriter::replace(Tag tag, std::string value, uint32_t index)
{
	fields[tag.field][index] = Field(tag, value);
}

void ProtoWriter::clear(Tag tag)
{
	return fields[tag.field].clear();
}

bool ProtoWriter::has(Tag tag)
{
	return fields[tag.field].size() > 0;
}

Field ProtoWriter::get(Tag tag)
{
	if (fields[tag.field].empty())
		return Field();
	return fields[tag.field][0];
}

std::vector<Field> ProtoWriter::getAll(Tag tag)
{
	return fields[tag.field];
}

#pragma endregion