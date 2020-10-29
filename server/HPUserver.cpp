/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include "habana.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using habana::AIP;
using habana::DriverVersion;
using habana::PCIeInfo;
using habana::MACaddress;
using habana::HostEngine;
using std::chrono::system_clock;


class HostEngineImpl final : public HostEngine::Service {
 public:
    explicit HostEngineImpl(const std::string &name) {
	    std::cout << "habana::HostEngine Service begins." <<name << std::endl;
      MACaddress_list_.clear();
      InitMacList(MACaddress_list_);
  }

  void InitMacList(std::vector<MACaddress> & Macaddress_list_)
  {
      Macaddress_list_.clear();
      for ( int i = 0; i< 10; i++) {
          Macaddress_list_.push_back(MACaddress());
          std::string str  = "Mac Address " + std::to_string(i);
          Macaddress_list_.back().set_maddress(str.c_str());  
      }
  }
#if 0
  Status GetDriverVer(ServerContext* context, const AIP* chipaip,
                    DriverVersion* driverver) override {
    driverver->set_driverversion("driver version. 0.1");
    return Status::OK;
  }
#endif
  
 Status ListPCI(ServerContext *context, const AIP * chipaip,
		PCIeInfo *pciinfo) override {
     pciinfo->set_bus(0);
     pciinfo->set_device(10);
     pciinfo->set_domain(20);
     pciinfo->set_deviceid(" Device Id is: 0x00ff000");
     return Status::OK;
     }

 Status ListMACaddress(ServerContext *context, const AIP *chipaip,
		 ServerWriter<MACaddress>* writer) override {
    for (const MACaddress &m: MACaddress_list_) {
      writer->Write(m);
    }  
	 return Status::OK;
 }

private:
   std::vector<MACaddress> MACaddress_list_;


};


void RunServer(const std::string& servername) {
  std::string server_address("0.0.0.0:50051");
  HostEngineImpl service(servername);

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer("Habana server");
  return 0;
}



#if 0


float ConvertToRadians(float num) {
  return num * 3.1415926 /180;


// The formula is based on http://mathforum.org/library/drmath/view/51879.html
float GetDistance(const Point& start, const Point& end) {
  const float kCoordFactor = 10000000.0;
  float lat_1 = start.latitude() / kCoordFactor;
  float lat_2 = end.latitude() / kCoordFactor;
  float lon_1 = start.longitude() / kCoordFactor;
  float lon_2 = end.longitude() / kCoordFactor;
  float lat_rad_1 = ConvertToRadians(lat_1);
  float lat_rad_2 = ConvertToRadians(lat_2);
  float delta_lat_rad = ConvertToRadians(lat_2-lat_1);
  float delta_lon_rad = ConvertToRadians(lon_2-lon_1);

  float a = pow(sin(delta_lat_rad/2), 2) + cos(lat_rad_1) * cos(lat_rad_2) *
            pow(sin(delta_lon_rad/2), 2);
  float c = 2 * atan2(sqrt(a), sqrt(1-a));
  int R = 6371000; // metres

  return R * c;
}

std::string GetFeatureName(const Point& point,
                           const std::vector<Feature>& feature_list) {
  for (const Feature& f : feature_list) {
    if (f.location().latitude() == point.latitude() &&
        f.location().longitude() == point.longitude()) {
      return f.name();
    }
  }
  return "";
}

class HostEngineImpl final : public HostEngine::Service {
 public:
  explicit HostEngineImpl(const std::string& db) {
    habana::ParseDb(db, &feature_list_);
  }

  Status GetFeature(ServerContext* context, const Point* point,
                    Feature* feature) override {
    feature->set_name(GetFeatureName(*point, feature_list_));
    feature->mutable_location()->CopyFrom(*point);
    return Status::OK;
  }

  Status ListFeatures(ServerContext* context,
                      const habana::Rectangle* rectangle,
                      ServerWriter<Feature>* writer) override {
    auto lo = rectangle->lo();
    auto hi = rectangle->hi();
    long left = (std::min)(lo.longitude(), hi.longitude());
    long right = (std::max)(lo.longitude(), hi.longitude());
    long top = (std::max)(lo.latitude(), hi.latitude());
    long bottom = (std::min)(lo.latitude(), hi.latitude());
    for (const Feature& f : feature_list_) {
      if (f.location().longitude() >= left &&
          f.location().longitude() <= right &&
          f.location().latitude() >= bottom &&
          f.location().latitude() <= top) {
        writer->Write(f);
      }
    }
    return Status::OK;
  }

  Status RecordRoute(ServerContext* context, ServerReader<Point>* reader,
                     RouteSummary* summary) override {
    Point point;
    int point_count = 0;
    int feature_count = 0;
    float distance = 0.0;
    Point previous;

    system_clock::time_point start_time = system_clock::now();
    while (reader->Read(&point)) {
      point_count++;
      if (!GetFeatureName(point, feature_list_).empty()) {
        feature_count++;
      }
      if (point_count != 1) {
        distance += GetDistance(previous, point);
      }
      previous = point;
    }
    system_clock::time_point end_time = system_clock::now();
    summary->set_point_count(point_count);
    summary->set_feature_count(feature_count);
    summary->set_distance(static_cast<long>(distance));
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(
        end_time - start_time);
    summary->set_elapsed_time(secs.count());

    return Status::OK;
  }

  Status RouteChat(ServerContext* context,
                   ServerReaderWriter<RouteNote, RouteNote>* stream) override {
    RouteNote note;
    while (stream->Read(&note)) {
      std::unique_lock<std::mutex> lock(mu_);
      for (const RouteNote& n : received_notes_) {
        if (n.location().latitude() == note.location().latitude() &&
            n.location().longitude() == note.location().longitude()) {
          stream->Write(n);
        }
      }
      received_notes_.push_back(note);
    }

    return Status::OK;
  }

 private:
  std::vector<Feature> feature_list_;
  std::mutex mu_;
  std::vector<RouteNote> received_notes_;
};

#endif

