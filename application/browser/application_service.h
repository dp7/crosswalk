// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_APPLICATION_BROWSER_APPLICATION_SERVICE_H_
#define XWALK_APPLICATION_BROWSER_APPLICATION_SERVICE_H_

#include <string>
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "xwalk/application/browser/application.h"
#include "xwalk/application/common/permission_policy_manager.h"
#include "xwalk/runtime/browser/runtime_context.h"
#include "xwalk/application/common/application_data.h"

namespace xwalk {

class RuntimeContext;

namespace application {

class ApplicationStorage;
class PackageInstaller;

// The application service manages install, uninstall and updates of
// applications.
class ApplicationService : public Application::Observer {
 public:
  // Client code may use this class (and register with AddObserver below) to
  // keep track of [un]installation of applications.
  class Observer {
   public:
    virtual void DidLaunchApplication(Application* app) {}
    virtual void WillDestroyApplication(Application* app) {}
   protected:
    virtual ~Observer() {}
  };

  ApplicationService(RuntimeContext* runtime_context,
                     ApplicationStorage* app_storage);
  virtual ~ApplicationService();

  Application* Launch(scoped_refptr<ApplicationData> application_data,
                      const Application::LaunchParams& launch_params);
  // Launch an installed application using application id.
  Application* Launch(
      const std::string& id,
      const Application::LaunchParams& params = Application::LaunchParams());
  // Launch an unpacked application using path to a local directory which
  // contains manifest file.
  Application* Launch(
      const base::FilePath& path,
      const Application::LaunchParams& params = Application::LaunchParams());

  Application* GetApplicationByRenderHostID(int id) const;
  Application* GetApplicationByID(const std::string& app_id) const;

  const ScopedVector<Application>& active_applications() const {
      return applications_; }

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  // Check whether application has permission to access API of extension.
  void CheckAPIAccessControl(const std::string& app_id,
      const std::string& extension_name,
      const std::string& api_name, const PermissionCallback& callback);
  // Register APIs implemented by extension. This method will be called
  // when application register extensions.
  // Parameter perm_table is a string which is a map between extension
  // and it includes APIs. For example perm_table is like '{"bluetooth":
  // ["read", "write", "management"]}'.
  bool RegisterPermissions(const std::string& app_id,
      const std::string& extension_name,
      const std::string& perm_table);

 private:
  // Implementation of Application::Observer.
  virtual void OnApplicationTerminated(Application* app) OVERRIDE;

  xwalk::RuntimeContext* runtime_context_;
  ApplicationStorage* application_storage_;
  ScopedVector<Application> applications_;
  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(ApplicationService);
};

}  // namespace application
}  // namespace xwalk

#endif  // XWALK_APPLICATION_BROWSER_APPLICATION_SERVICE_H_
