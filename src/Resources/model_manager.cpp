#include "Vocksel/Resources/model_manager.h"

Vocksel::ModelManager::ModelManager() {

}

void Vocksel::ModelManager::loadModel(const std::string &path, const std::string &name) {
    auto model = std::make_unique<Model>();
    model->loadFromFile(path);
    models_[name] = std::move(model);
}

bool Vocksel::ModelManager::createModelFromData(const std::string &name, const float *vertices, size_t vertexCount, const uint32_t *indices, size_t indexCount, int vertexStride) {
    if (getModel(name)) {
        std::cerr << "Model " << name << " already exists\n";
        return false;
    }

    auto model = std::make_unique<Model>();
    auto mesh = std::make_unique<StaticMesh>(vertices, vertexCount, indices, indexCount, vertexStride);
    model->addMesh(std::move(mesh));
    models_[name] = std::move(model);
    return true;
}


Vocksel::Model* Vocksel::ModelManager::getModel(const std::string &name) {
    auto it = models_.find(name);
    if (it != models_.end()) {
        return it->second.get();
    }
    return nullptr;
}


void Vocksel::ModelManager::cleanUp() {
    models_.clear();
}

Vocksel::ModelManager::~ModelManager() {
    cleanUp();
}



