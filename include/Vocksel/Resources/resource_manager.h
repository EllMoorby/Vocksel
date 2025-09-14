#ifndef RESOURCE__MANAGER_H
#define RESOURCE__MANAGER_H
#include <functional>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "Vocksel/Graphics/Mesh/model.h"
#include "Vocksel/Graphics/shader.h"
#include "Vocksel/Graphics/texture.h"
#include "Vocksel/Graphics/texture_atlas.h"

namespace Vocksel {

class ResourceManager {
 public:
  ResourceManager();
  ~ResourceManager();

  // Prevent copying
  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager&) = delete;

  void init();

  void reloadShader(std::string name);
  void cleanUp();

  // Generic resource access
  template <typename T>
  void load(const std::string& name, auto&&... args);

  template <typename T>
  T& get(const std::string& name);

  template <class T>
  T* tryGet(const std::string& name);

  template <typename T>
  const T& get(const std::string& name) const;

  template <typename T>
  std::vector<std::string> getNames();

 private:
  void initDebug();

  // Internal resource map handling
  template <typename T>
  using ResourceMap = std::unordered_map<std::string, std::unique_ptr<T>>;

  template <typename T>
  ResourceMap<T>& getResourceMap();

  using ErasedMapPtr = std::unique_ptr<void, std::function<void(void*)>>;
  std::unordered_map<std::type_index, ErasedMapPtr> resource_maps_;

  bool initialized_ = false;
};
}  // namespace Vocksel

template <typename T>
void Vocksel::ResourceManager::load(const std::string& name, auto&&... args) {
  if constexpr (std::is_same_v<T, Shader>) {
    // Handle shader loading directly
    auto& map = getResourceMap<Shader>();

    if constexpr (sizeof...(args) == 1) {
      // Compute shader - single argument
      map[name] =
          std::make_unique<Shader>(std::forward<decltype(args)>(args)...);
    } else if constexpr (sizeof...(args) == 2) {
      // Vertex + fragment shader - two arguments
      map[name] =
          std::make_unique<Shader>(std::forward<decltype(args)>(args)...);
    } else {
      static_assert(
          sizeof(T) == 0,
          "Shader requires 1 (compute) or 2 (vertex+fragment) arguments");
    }
  } else {
    // For other types, create directly
    auto& map = getResourceMap<T>();
    map[name] = std::make_unique<T>(std::forward<decltype(args)>(args)...);
  }
}

template <typename T>
T& Vocksel::ResourceManager::get(const std::string& name) {
  auto& map = getResourceMap<T>();
  auto it = map.find(name);
  if (it == map.end()) {
    throw std::runtime_error("Resource not found: " + name);
  }
  return *it->second;
}

template <typename T>
T* Vocksel::ResourceManager::tryGet(const std::string& name) {
  auto& map = getResourceMap<T>();
  auto it = map.find(name);
  if (it == map.end()) {
    return nullptr;
  }
  return it->second.get();  // return raw pointer from unique_ptr
}

template <typename T>
const T& Vocksel::ResourceManager::get(const std::string& name) const {
  return const_cast<ResourceManager*>(this)->get<T>(name);
}

template <typename T>
Vocksel::ResourceManager::ResourceMap<T>&
Vocksel::ResourceManager::getResourceMap() {
  // Compile-time check - prevents invalid types entirely
  if constexpr (!std::is_same_v<T, Shader> && !std::is_same_v<T, Texture> &&
                !std::is_same_v<T, TextureAtlas> && !std::is_same_v<T, Model>) {
    static_assert(
        sizeof(T) == 0,
        "Unsupported resource type. Add to whitelist in ResourceManager.");
  }

  static auto type_id = std::type_index(typeid(T));
  auto it = resource_maps_.find(type_id);
  if (it == resource_maps_.end()) {
    // allocate the concrete ResourceMap<T>
    auto* new_map = new ResourceMap<T>();
    // create an ErasedMapPtr with a deleter that deletes ResourceMap<T>
    // correctly
    ErasedMapPtr uptr(new_map,
                      [](void* p) { delete static_cast<ResourceMap<T>*>(p); });
    it = resource_maps_.emplace(type_id, std::move(uptr)).first;
  }
  return *static_cast<ResourceMap<T>*>(it->second.get());
}

template <typename T>
std::vector<std::string> Vocksel::ResourceManager::getNames() {
  if (!initialized_)
    throw std::runtime_error("ResourceManager not initialized");

  auto& map = getResourceMap<T>();
  std::vector<std::string> names;
  names.reserve(map.size());

  for (auto& [name, texture] : map) {
    names.push_back(name);
  }
  return names;
}

#endif  // RESOURCE__MANAGER_H
