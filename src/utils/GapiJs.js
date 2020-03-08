export let listFiles = () => {
  return gapi.client.drive.files
    .list({
      pageSize: 10,
      fields: "nextPageToken, files(id, name)"
    })
    .then(function(response) {
      var files = response.result.files;

      if (files && files.length > 0) {
        const filenames = files.map(file => {
          return file.name + " (" + file.id + ")";
        });

        return Promise.resolve(filenames);
      } else {
        return Promise.reject("No files found.");
      }
    });
};
