#pragma once

#define VULKAN_CLASSES_DISABLE_COPY_AND_MOVE(Type)\
	Type(const Type&) = delete;\
	Type(Type&&) = delete;\
	Type& operator=(const Type&) = delete;\
	Type& operator=(Type&&) = delete;

#define VULKAN_CLASSES_DEFAULT_COPY_AND_MOVE(Type)\
	Type(const Type&) = default;\
	Type(Type&&) noexcept = default;\
	Type& operator=(const Type&) = default;\
	Type& operator=(Type&&) noexcept = default;
