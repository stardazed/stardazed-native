// ------------------------------------------------------------------
// model::LWOLoader.cpp
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "LWOLoader.hpp"
#include "system/Logging.hpp"
#include "render/common/VertexBuffer.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <unordered_set>
#include <unordered_map>

using namespace sd;
using namespace std::string_literals;


struct LWVertex {
	int32 vertexIndex, positionIndex, normalIndex, uvIndex;
};


class LWDelegate {
public:
	virtual ~LWDelegate() = default;
	
	virtual void counts(size32 positionCount, size32 normalCount, size32 uvCount, size32 faceCount, size32 vertexCount) = 0;
	
	virtual void material(const model::StandardMaterialDescriptor&) = 0;
	
	virtual void position(float x, float y, float z) = 0;
	virtual void normal(float x, float y, float z) = 0;
	virtual void uv(float s, float t) = 0;

	virtual void useMaterialName(const std::string&) = 0;
	virtual void face(const std::string& a, const std::string& b, const std::string& c) = 0;
	
	virtual void complete() = 0;
};



class MTLParser {
	model::StandardMaterialDescriptor mat_;
	
	void parseMaterialName(std::istringstream& tokens, LWDelegate& delegate) {
		materialComplete(delegate);

		std::string name;
		tokens >> name;
	}
	
	void parseColour(char kind, std::istringstream& tokens, LWDelegate&) {
		float r, g, b;
		tokens >> r >> g >> b;
		
		switch (kind) {
			case 'a': break;
			case 'd': mat_.mainColour = { r, g, b }; break;
			case 's': mat_.specularColour = { r, g, b }; break;
			default:
//				log("unknown colour component ", std::string{1, kind});
				break;
		}
	}
	
	void parseSpecularExp(std::istringstream& tokens, LWDelegate&) {
		float specExp;
		tokens >> specExp;
		mat_.specularExponent = specExp;
	}

	void parseDiffuseMap(std::istringstream& tokens, LWDelegate&) {
		std::string name;
		tokens >> name;
		mat_.diffuseMapFileName = name;
	}
	
	void materialComplete(LWDelegate& delegate) {
		if (mat_.name.length() == 0)
			return;
		
		delegate.material(mat_);

		auto nextIx = mat_.index + 1;
		mat_ = Material();
		mat_.index = nextIx;
	}

public:
	MTLParser(std::istream& mtlFile, LWDelegate& delegate) {
		std::string line, cmd;
		
		const auto matNameCmd = "newmtl"s,
		           specExpCmd = "Ns"s,
				   diffuseMapCmd = "map_Kd"s;
		
		while (std::getline(mtlFile, line)) {
			if (line.length() == 0)
				continue;

			std::istringstream tokens {line};
			tokens >> cmd;
			
			if (cmd[0] == 'K' && cmd.length() == 2)
				parseColour(cmd[1], tokens, delegate);
			else if (cmd == matNameCmd)
				parseMaterialName(tokens, delegate);
			else if (cmd == specExpCmd)
				parseSpecularExp(tokens, delegate);
			else if (cmd == diffuseMapCmd)
				parseDiffuseMap(tokens, delegate);
//			else
//				sd::log("skip mtl cmd ", cmd);
		}

		// last material in file
		materialComplete(delegate);
	}
};



class LWOParser {
	const std::string matFileCmd = "mtllib",
		matRefCmd = "usemtl",
		positionCmd = "v",
		normalCmd = "vn",
		uvCmd = "vt",
		faceCmd = "f";

	void parseMaterialFileName(std::istringstream& tokens, LWDelegate& delegate) {
		std::string materialFileName;
		tokens >> materialFileName;

		std::ifstream mtlFile { "data/models/" + materialFileName };
		MTLParser(mtlFile, delegate);
	}
	
	void parseMaterialRef(std::istringstream& tokens, LWDelegate& delegate) {
		std::string matName;
		tokens >> matName;
		delegate.useMaterialName(matName);
	}
	
	void parsePosition(std::istringstream& tokens, LWDelegate& delegate) {
		float x, y, z;
		tokens >> x >> y >> z;
		delegate.position(x, y, z);
	}
	
	void parseNormal(std::istringstream& tokens, LWDelegate& delegate) {
		float x, y, z;
		tokens >> x >> y >> z;
		delegate.normal(x, y, z);
	}
	
	void parseUV(std::istringstream& tokens, LWDelegate& delegate) {
		float s, t;
		tokens >> s >> t;
		delegate.uv(s, t);
	}

	void parseFace(std::istringstream& tokens, LWDelegate& delegate) {
		std::string a, b, c, d;
		tokens >> a >> b >> c;

		delegate.face(a, b, c);

		if (tokens >> d) {
			delegate.face(a, c, d);
		}
	}

	void preflight(std::istream& objFile, LWDelegate& delegate) {
		std::string line, cmd, vtx;
		size32 positionCount = 0, normalCount = 0, uvCount = 0, faceCount = 0;
		std::unordered_set<std::string> uniques_;
		uniques_.reserve(1024);

		while (std::getline(objFile, line)) {
			std::istringstream tokens {line};
			tokens >> cmd;
			
			if (cmd == positionCmd)
				++positionCount;
			else if (cmd == normalCmd)
				++normalCount;
			else if (cmd == uvCmd)
				++uvCount;
			else if (cmd == faceCmd) {
				// generally either 3 or 4 indexes, which amount to 1 or 2 faces
				tokens >> vtx;
				uniques_.insert(vtx);
				tokens >> vtx;
				uniques_.insert(vtx);
				while (tokens >> vtx) {
					uniques_.insert(vtx);
					++faceCount;
				}
			}
		}
		
		delegate.counts(positionCount, normalCount, uvCount, faceCount, (size32)uniques_.size());
	}
	
	void parse(std::istream& objFile, LWDelegate& delegate) {
		std::string line, cmd;
		
		while (std::getline(objFile, line)) {
			std::istringstream tokens {line};
			tokens >> cmd;
			
			if (cmd == positionCmd)
				parsePosition(tokens, delegate);
			if (cmd == normalCmd)
				parseNormal(tokens, delegate);
			else if (cmd == uvCmd)
				parseUV(tokens, delegate);
			else if (cmd == faceCmd)
				parseFace(tokens, delegate);
			else if (cmd == matRefCmd)
				parseMaterialRef(tokens, delegate);
			else if (cmd == matFileCmd)
				parseMaterialFileName(tokens, delegate);
//			else
//				sd::log("skip cmd ", cmd);
		}
	}

public:
	LWOParser(std::istream& objFile, LWDelegate& delegate) {
		preflight(objFile, delegate);
		objFile.clear();
		objFile.seekg(0);
		parse(objFile, delegate);
		delegate.complete();
	}
};



class LWModelBuilder : public LWDelegate {
	Model& model_;
	std::unordered_map<std::string, LWVertex> vertexMapping_;

	std::vector<math::Vec3> positions_, normals_;
	std::vector<math::Vec2> uvs_;
	
	std::vector<math::Vec3>::iterator tempPosIt_, tempNormIt_;
	std::vector<math::Vec2>::iterator tempUVIt_;
	size32 faceCount_ = 0;

	render::VertexBuffer::AttrIterator<math::Vec3> posIter_, normIter_;
	render::VertexBuffer::AttrIterator<math::Vec2> uvIter_;

	int curMaterialIx_ = -1;
	size32 matGroupStartFaceIx_ = 0, curFaceIx_ = 0;

public:
	LWModelBuilder(Model& model)
	: model_{model}
	{}
	
	void counts(size32 positionCount, size32 normalCount, size32 uvCount, size32 faceCount, size32 vertexCount) override {
		using namespace render;

		log("pos: ", positionCount, " nrm: ", normalCount, " uv: ", uvCount, " face: ", faceCount, " vtx: ", vertexCount);
		
		positions_.resize(positionCount);
		if (normalCount > 0)
			normals_.resize(normalCount);
		uvs_.resize(uvCount);
		
		tempPosIt_ = positions_.begin();
		tempNormIt_ = normals_.begin();
		tempUVIt_ = uvs_.begin();
		faceCount_ = faceCount;

		model_.meshData.primaryVertexBuffer().allocate(vertexCount);
		posIter_ = model_.meshData.primaryVertexBuffer().attrBegin<math::Vec3>(VertexAttributeRole::Position);
		normIter_ = model_.meshData.primaryVertexBuffer().attrBegin<math::Vec3>(VertexAttributeRole::Normal);
		uvIter_ = model_.meshData.primaryVertexBuffer().attrBegin<math::Vec2>(VertexAttributeRole::UV);

		auto indexElementType = minimumIndexElementTypeForVertexCount(vertexCount);
		model_.meshData.indexBuffer.allocate(PrimitiveType::Triangle, indexElementType, faceCount);
	}

	void material(const Material& mat) override {
		model_.materials.push_back(mat);
	}

	void position(float x, float y, float z) override {
		*tempPosIt_++ = { x, y, z };
	}
	
	void normal(float x, float y, float z) override {
		// normals from obj files are not guaranteed to be normalized...
		*tempNormIt_++ = math::normalize(math::Vec3{ x, y, z });
	}
	
	void uv(float s, float t) override {
		*tempUVIt_++ = { s, t };
	}

	LWVertex parseFaceVertex(const std::string& vertex) {
		LWVertex result { -1, -1, -1, -1 };
		int offset = 0, len = (int)vertex.length();
		
		auto chr = [&](int at) {
			if (at >= 0 && at < len)
				return vertex[at];
			return '\0';
		};
		
		auto num = [&]{
			int endOffset = offset;
			int val = -1;
			while (isdigit(chr(endOffset)))
				++endOffset;
			if (endOffset > offset)
				val = std::stoi(vertex.substr(offset, endOffset - offset)) - 1;
			offset = endOffset;
			return val;
		};
		
		result.positionIndex = num();
		if (chr(offset) == '/') {
			++offset;
			result.uvIndex = num();
			if (chr(offset) == '/') {
				++offset;
				result.normalIndex = num();
			}
		}
		
		return result;
	}
	
	const LWVertex& getVertex(const std::string& vertexStr) {
		auto it = vertexMapping_.find(vertexStr);
		if (it != vertexMapping_.end())
			return it->second;

		// -- get the combination of position, normal and uv indexes
		auto vertex = parseFaceVertex(vertexStr);

		// -- place the data into the vertexbuffer
		*posIter_++ = positions_[vertex.positionIndex];
		if (vertex.normalIndex > -1)
			*normIter_ = normals_[vertex.normalIndex];
		++normIter_;
		if (vertex.uvIndex > -1)
			*uvIter_ = uvs_[vertex.uvIndex];
		++uvIter_;

		// -- set the vertex index to the current position in the vertexbuffer
		vertex.vertexIndex = (int32)vertexMapping_.size();
		vertexMapping_.insert({ vertexStr, vertex });
		return vertexMapping_[vertexStr];
	}
	
	void face(const std::string& a, const std::string& b, const std::string& c) override {
		const LWVertex& vA = getVertex(a);
		const LWVertex& vB = getVertex(b);
		const LWVertex& vC = getVertex(c);

		render::IndexBufferTriangleView triangleView{ model_.meshData.indexBuffer };
		triangleView.setTriangle(curFaceIx_++, (uint32)vA.vertexIndex, (uint32)vB.vertexIndex, (uint32)vC.vertexIndex);
	}
	
	void finishMaterialGroup() {
		if (curMaterialIx_ < 0)
			return;

		model_.faceGroups.push_back({matGroupStartFaceIx_, curFaceIx_, size32_cast(curMaterialIx_)});
		
		matGroupStartFaceIx_ = curFaceIx_;
		curMaterialIx_ = -1;
	}
	
	int materialIndexByName(const std::string& name) {
		auto it = std::find_if(model_.materials.begin(), model_.materials.end(), [&name](const Material& mat){
			return mat.name == name;
		});
		
		if (it == model_.materials.end())
			return -1;
		return static_cast<int>(it - std::begin(model_.materials));
	}

	void useMaterialName(const std::string& name) override {
		finishMaterialGroup();
	
		curMaterialIx_ = materialIndexByName(name);
		if (curMaterialIx_ < 0)
			sd::log("Trying to use non-existent material ", name);
	}

	void complete() override {
		finishMaterialGroup();
		if (normals_.size() == 0)
			model_.meshData.genVertexNormals();
	}
};


Model* newLWOModelFromPath(const std::string& filePath) {
	Model* model = new Model{};
	model->meshData.vertexBuffers.emplace_back(render::AttrList::Pos3Norm3UV2Tan4());
	
	auto builder = std::make_unique<LWModelBuilder>(*model);
	std::ifstream file { filePath };
	if (file)
		LWOParser parser { file, *builder.get() };
	else
		sd::log("Can't open file ", filePath);

	sd::log("materials: ", model->materials.size(), " groups: ", model->faceGroups.size());
	
	return model;
}
