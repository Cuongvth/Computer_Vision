import { HTTP } from "@/api/httpOCR";

export function checkInData(id) {
  return new Promise((resolve, reject) => {
    HTTP.get(`nhandang?duLieuId=${id}`)
      .then(response => {
        resolve(response.data);
      })
      .catch(error => {
        reject(error);
      });
  });
}